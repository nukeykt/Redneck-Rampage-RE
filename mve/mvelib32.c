#include <stdio.h>
#include <dos.h>
#include <string.h>
#include "sos.h"
#include "dpmi.h"
#include "mvelib32.h"



typedef struct {
    void *f_0;
    unsigned int f_4;
    unsigned int f_8;
    unsigned int f_c;
} memstruct_t;

typedef struct {
    unsigned char f_0[20];
    unsigned short f_14;
    unsigned short f_16;
    unsigned short f_18;
    unsigned int f_1a;
} mveheader_t;

typedef struct {
    char *f_0;
    unsigned int f_4;
} sndqueue_t;

static mve_cb_alloc mem_alloc;
static mve_cb_free mem_free;
static unsigned int sync_period;
int sync_wait_quanta;
unsigned short prev_timer2;

static mve_cb_read io_read;
static int io_handle;
static unsigned int io_next_hdr;
static memstruct_t io_mem_buf;

static volatile unsigned int snd_cnt;
static sndqueue_t snd_queue[60];
static volatile unsigned int snd_empty;
static volatile unsigned int snd_processed;
static unsigned int snd_done;

static int snd_bits16;
static int snd_stereo;
static int snd_comp16;

static unsigned int snd_buf_base_len;
static unsigned int snd_buf_len;
static void *snd_buf;

static int snd_fill;
static volatile int snd_empty2;
static int snd_target_pad;
static unsigned int snd_buf_cur;
static int snd_total_processed;
static int snd_target;

static _SOS_SAMPLE snd_SampleData;

static memstruct_t snd_mem_buf;

static mve_cb_SoundHook snd_SoundHook;

static HANDLE snd_hDriver = -1;

static HANDLE sync_hTimer = -1;
volatile unsigned int sync_time = 0;
static int sync_late = 0;
static int sync_FrameDropped = 0;

// asm routines

void cdecl syncCallback(void);
void cdecl syncReset(unsigned int a1);
void cdecl syncInit2(void);
int cdecl syncMaybeWait(void);
unsigned int cdecl syncMaybeWaitLevel(unsigned int a1);
unsigned int cdecl syncTime(void);
unsigned int cdecl getDMAByteCnt(unsigned int a1);
void cdecl sndDecompM16(void *a1, void *a2, unsigned int a3);
void cdecl sndDecompS16(void *a1, void *a2, unsigned int a3);
void cdecl sndDecompM8(void *a1, void *a2, unsigned int a3);
void cdecl sndDecompS8(void *a1, void *a2, unsigned int a3);
void cdecl sndCnv16to8(void *a1, void *a2, unsigned int a3);
void cdecl nfHiColorDecomp(void *a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
void cdecl nfHiColorDecompChg(void *a1, void *a2, void *a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7);
void cdecl nfDecomp(void *a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
void cdecl nfDecompChg(void *a1, void *a2, void *a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7);
void cdecl nfPkConfig(void);
void cdecl nfPkDecomp(void* a1, void* a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
void cdecl nfPkDecompH(void* a1, void* a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
void cdecl nfHPkDecomp(void* a1, void* a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
void cdecl mve_ShowFrameField(void* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10);
void cdecl mve_ShowFrameFieldHi(void* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10);
void cdecl mve_sfShowFrameChg(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, void *a6, unsigned int a7, unsigned int a8);
void cdecl mve_sfHiColorShowFrameChg(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, void *a6, unsigned int a7, unsigned int a8);
void cdecl MVE_SetPalette(void *a1, unsigned int a2, unsigned int a3);
void cdecl palLoadCompPalette(void *a1);
void cdecl gfxMode(unsigned int a1);
void cdecl gfxLoadCrtc(void *a1, unsigned int a2, unsigned int a3);
void cdecl gfxGetCrtc(void *a1);
void cdecl gfxVres(unsigned int a1, void *a2);
void cdecl MVE_gfxWaitRetrace(unsigned int a1);
void cdecl MVE_gfxSetSplit(unsigned int a1);


void cdecl MVE_memCallbacks(mve_cb_alloc a1, mve_cb_free a2)
{
    mem_alloc = a1;
    mem_free = a2;
}

static void MemFree(memstruct_t *a1)
{
    if (a1->f_4 && a1->f_c)
    {
        dpmi_unlockmem(a1->f_0, a1->f_4);
    }
    if (a1->f_4 && mem_free)
    {
        mem_free(a1->f_0);
        a1->f_8 = 0;
    }
    a1->f_4 = 0;
}

static void MemInit(memstruct_t *a1, unsigned int a2, void *a3)
{
    if (!a3)
        return;
    MemFree(a1);
    a1->f_0 = a3;
    a1->f_4 = a2;
    a1->f_8 = 0;
    if (a1->f_c)
        dpmi_lockmem(a3, a2);
}

static void MemLock(memstruct_t *a1, unsigned int a2)
{
    if (a2)
    {
        if (!a1->f_c && a1->f_4)
            dpmi_lockmem(a1->f_0, a1->f_4);
    }
    else
    {
        if (!a1->f_c && a1->f_4)
            dpmi_unlockmem(a1->f_0, a1->f_4);
    }
    a1->f_c = a2;
}

static void *MemAlloc(memstruct_t *a1, unsigned int a2)
{
    void *p;
    if (a1->f_4 > a2)
        return a1->f_0;

    if (mem_alloc)
    {
        MemFree(a1);
        a2 += 100;
        p = mem_alloc(a2);
        if (p)
        {
            MemInit(a1, a2, p);
        }

        a1->f_8 = 1;
        return a1->f_0;
    }
    return NULL;
}

static void syncRelease(void)
{
    if (sync_hTimer != -1)
    {
        sosTIMERRemoveEvent(sync_hTimer);
        sync_hTimer = -1;
        sync_time = 0;
    }
}

static void syncCallbackF(void)
{
    syncCallback();
}

static int syncInit(unsigned int a1, int a2)
{
    static int dword_125FF0 = 1;
    static void *off_125FF4 = syncCallback;
    int v2 = -((a1 * 10000) / 8320 * a2);

    if (sync_hTimer == -1 || a1 != sync_period || v2 != sync_wait_quanta)
    {
        if (sync_hTimer != -1)
        {
            syncMaybeWait();
            sync_time += sync_wait_quanta;
        }
        sync_period = a1;
        sync_wait_quanta = v2;

        syncRelease();

        if (dword_125FF0)
        {
            dpmi_lockmem(&prev_timer2, sizeof(prev_timer2));
            dpmi_lockmem(&sync_time, sizeof(sync_time));
        }

        if (dword_125FF0)
        {
            dpmi_lockmem(off_125FF4, 1024);
            dpmi_lockmem(&syncCallbackF, 1024);
            dword_125FF0 = 0;
        }

        if (sosTIMERRegisterEvent(119, syncCallbackF, &sync_hTimer))
        {
            sync_hTimer = -1;
            return 0;
        }
        syncInit2();

    }
    return 1;
}

static int syncWait(void)
{
    int ret = 0;
    if (sync_hTimer != -1)
    {
        ret = syncMaybeWait();
        sync_time += sync_wait_quanta;
    }
    return ret;
}

static int syncWaitLevel(unsigned int a1)
{
    int ret = 0;
    if (sync_hTimer != -1)
    {
        ret = syncMaybeWaitLevel(a1);
        sync_time += sync_wait_quanta;
    }
    return ret;
}

static void syncSync(void)
{
    if (sync_hTimer != -1)
        syncMaybeWait();
}

void cdecl MVE_logDumpStats(void)
{
    printf("Logging support disabled.\n");
}

void cdecl MVE_ioCallbacks(mve_cb_read a1)
{
    io_read = a1;
}

static void *ioRead(int a1);

static int ioReset(int a1)
{
    mveheader_t *p;
    io_handle = a1;
    p = (mveheader_t*)ioRead(30);
    if (!p || strcmp(p->f_0, "Interplay MVE File\x1a") || p->f_18 != ~p->f_16 + 0x1234
        || p->f_16 != 0x100 || p->f_14 != 0x1a)
    {
        return 0;
    }

    io_next_hdr = p->f_1a;
    return 1;
}

void cdecl MVE_memIO(void *a1, unsigned int a2)
{
    MemInit(&io_mem_buf, a2, a1);
}

static void *ioRead(unsigned int a1)
{
    void *p = MemAlloc(&io_mem_buf, a1);
    if (!p)
        return NULL;
    if (!io_read(io_handle, p, a1))
        return NULL;
    return p;
}

static void *ioNextRecord(void)
{
    unsigned char *p = ioRead((unsigned short)io_next_hdr + 4);
    if (!p)
        return NULL;
    io_next_hdr = *(unsigned int*)(p + (unsigned short)io_next_hdr);
    return p;
}

static void ioRelease(void)
{
    MemFree(&io_mem_buf);
}

W32 sosDIGIGetBytesPlayed(HANDLE a1, HANDLE a2)
{
    _SOS_DIGI_DRIVER *v1;
    _SOS_SAMPLE *v2;
    DWORD v20;
    DWORD v28;
    DWORD v1c;
    DWORD v18;
    WORD v24;
    WORD v14;
    WORD t;
    DWORD t2;
    DWORD t3;
    DWORD t4;
    DWORD t5;
    DWORD t6;
    v1 = _sSOSSystem.pDriver[a1];
    if (!v1)
        return 0;
    v2 = &v1->pSamples[a2];
    if (!v2)
        return 0;

    if (!(v2->wFlags & 0x8000))
        return 0;

    v20 = v1->wDMABufferSize;
    v28 = v1->wDMACountRegister;

    _disable();

    v1c = v2->wTotalBytesProcessed;

    v18 = v1->pXFERPosition - v1->pDMABuffer;

    v14 = getDMAByteCnt(v28);
    t = getDMAByteCnt(v28);
    if (v14 < t)
        v14 = t;
    _enable();

    t2 = v20 - v14;
    if (t2 <= v18)
    {
        t2 = v18 - t2;
    }
    else
    {
        t2 = v18 + v20 - t2;
    }

    t4 = v2->wRate;
    if (v2->wChannels == 2)
        t4 *= 2;
    if (v2->wBitsPerSample == 16)
        t4 *= 2;

    t5 = v1->wDriverRate;
    if (v1->wDriverChannels == 2)
        t5 *= 2;
    if (v1->wDriverBitsPerSample == 16)
        t5 *= 2;

    t2 = (t2 * t4) / t5;

    if (t2 > v1c)
        return 0;

    return v1c - t2;
}

static HANDLE snd_SampleHandle = -1;
static int snd_paused = 0;
static int snd_vol = 0x7fff7fff;

static void cdecl snd_SampleCallback(_SOS_SAMPLE *a1)
{
    if (!snd_paused && snd_cnt > 0 && snd_SampleHandle != -1)
    {
        a1->pSample = snd_queue[snd_empty].f_0;
        a1->wLength = snd_queue[snd_empty].f_4;
        snd_processed += a1->wLength;
        if (snd_empty == 59)
            snd_empty = 0;
        else
            ++snd_empty;
        --snd_cnt;
    }
    else
        snd_SampleHandle = -1;
}

static void cdecl snd_SampleCallback2(_SOS_SAMPLE *a1)
{
    snd_done = 1;
}

void cdecl MVE_AIL_sndInit(void)
{
    snd_hDriver = -1;
}

void cdecl MVE_SOS_sndInit(int a1)
{
    snd_hDriver = a1;
}

unsigned int cdecl MVE_sndVolume(unsigned int a1)
{
    unsigned int ret = snd_vol;

    snd_vol = a1 & 0x7fff7fff;

    return ret;
}

void cdecl MVE_rmemSND(void)
{
}

static void sndReset(void)
{
    int handle;
    if (snd_hDriver == -1)
        return;
    handle = snd_SampleHandle;
    if (handle == -1)
        return;
    sosDIGIStopSample(snd_hDriver, handle);
    snd_SampleHandle = -1;
}

static int sndConfigure(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned a5, unsigned int a6)
{
    static int dword_126004 = 1;
    _SOS_CAPABILITIES caps;
    if (snd_hDriver == -1)
        return 1;
    syncSync();
    sndReset();
    snd_stereo = a3;
    snd_bits16 = a5;
    snd_comp16 = a6;
    if (a3)
    {
        sosDIGIGetDeviceCaps(snd_hDriver, &caps);
        if (caps.wChannels == 0)
        {
            a4 <<= 1;
            a3 = 0;
        }
    }
    memset(&snd_SampleData, 0, sizeof(snd_SampleData));

    snd_SampleData.wChannels = a3 ? 2 : 1;
    snd_SampleData.wFormat = a5 ? 0 : 0x8000;
    snd_SampleData.wBitsPerSample = a5 ? 16 : 8;
    snd_SampleData.wRate = a4;
    snd_SampleData.wVolume = snd_vol;
    snd_SampleData.wPanPosition = 0x8000;
    snd_SampleData.pfnSampleProcessed = snd_SampleCallback;
    snd_SampleData.pfnSampleDone = snd_SampleCallback2;
    snd_buf_base_len = a2;
    snd_buf_len = (a2 >> 1) + a2;

    snd_buf = MemAlloc(&snd_mem_buf, snd_buf_len);

    if (!snd_buf)
        return 0;
    MemLock(&snd_mem_buf, 1);
    if (dword_126004)
    {
        dpmi_lockmem(snd_SampleCallback, 4096);
        dpmi_lockmem(snd_SampleCallback2, 4096);
        dpmi_lockmem(&snd_SampleData, sizeof(snd_SampleData));
        dpmi_lockmem(snd_queue, sizeof(snd_queue));
        dpmi_lockmem(&snd_processed, sizeof(snd_processed));
        dpmi_lockmem(&snd_empty, sizeof(snd_empty));
        dpmi_lockmem(&snd_hDriver, sizeof(snd_hDriver));
        dpmi_lockmem(&snd_SampleHandle, sizeof(snd_SampleHandle));
        dpmi_lockmem(&snd_done, sizeof(snd_done));
        dword_126004 = 0;
    }

    snd_fill = 0;
    snd_empty = 0;
    snd_empty2 = 0;
    snd_cnt = 0;
    snd_target_pad = 0;
    snd_buf_cur = 0;
    snd_paused = 0;
    snd_processed = 0;
    snd_total_processed = 0;
    snd_done = 1;
    snd_target = 0;

    return 1;
}

static void sndSync(void)
{
    int v1;
    int v2;
    unsigned int v3;
    unsigned int handle;
    v1 = syncWaitLevel(sync_wait_quanta >> 2);

    sync_late = v1 > (-sync_wait_quanta >> 1) && !sync_FrameDropped;
    sync_FrameDropped = 0;

    if (snd_hDriver == -1)
        return;

    if (!snd_target_pad)
        snd_target_pad = snd_queue[0].f_4;

    v2 = 0;
    while (1)
    {
        handle = snd_SampleHandle;
        if (handle != -1 ? 1 : 0)
        {
            v3 = sosDIGIGetBytesPlayed(snd_hDriver, handle);
            if (v3 + snd_total_processed + snd_target_pad < snd_target)
                v2 = 1;
            else
                break;
        }
        else
            break;
    }
    if (v2)
    {
        syncReset(sync_wait_quanta + (sync_wait_quanta >> 2));
    }

    handle = snd_SampleHandle;
    if (handle != -1 ? 1 : 0)
    {
        v3 = sosDIGIGetBytesPlayed(snd_hDriver, handle);
        if (v3 + snd_total_processed > snd_target + snd_target_pad)
            snd_SampleHandle = -1;
    }

    if (snd_done && snd_SampleHandle == -1 && snd_cnt
        && snd_total_processed + snd_processed <= snd_target)
    {
        snd_SampleData.pSample = snd_queue[snd_empty].f_0;
        snd_SampleData.wLength = snd_queue[snd_empty].f_4;
        snd_total_processed += snd_processed;
        snd_processed = snd_SampleData.wLength;
        if (snd_empty == 59)
            snd_empty = 0;
        else
            ++snd_empty;
        --snd_cnt;
        snd_done = 0;
        snd_SampleHandle = sosDIGIStartSample(snd_hDriver, &snd_SampleData);
        while (1)
        {
            handle = snd_SampleHandle;
            if (handle != -1 ? 1 : 0)
            {
                if (sosDIGIGetBytesPlayed(snd_hDriver, handle) == 0 ? 1 : 0)
                {
                }
                else
                    break;
            }
            else
                break;
        }

        syncReset(sync_wait_quanta);
    }
    if (snd_empty2 != snd_fill)
    {
        snd_target += snd_queue[snd_empty2].f_4;
        if (snd_empty2 == 59)
            snd_empty2 = 0;
        else
            ++snd_empty2;
    }
}

void cdecl MVE_sndSoundHook(mve_cb_SoundHook a1)
{
    snd_SoundHook = a1;
}

static void sndAdd(void *a1, unsigned int a2)
{
    if (snd_hDriver == -1 || snd_fill + 1 == (snd_empty2 ? snd_empty2 : 60))
        return;

    snd_paused = 0;
    if (snd_buf_cur + a2 > snd_buf_len)
        snd_buf_cur = 0;

    if (a1 && snd_SoundHook)
    {
        snd_SoundHook((unsigned char*)snd_buf + snd_buf_cur, a2, 1, snd_bits16);
    }
    else if (a1)
    {
        if (snd_comp16)
        {
            if (snd_stereo)
                sndDecompS16((unsigned char*)snd_buf + snd_buf_cur, a1, a2);
            else
                sndDecompM16((unsigned char*)snd_buf + snd_buf_cur, a1, a2);
        }
        else
        {
            memcpy((unsigned char*)snd_buf + snd_buf_cur, a1, a2);
        }
    }
    else
    {
        memset((unsigned char*)snd_buf + snd_buf_cur, snd_bits16 ? 0 : 128, a2);
    }

    snd_queue[snd_fill].f_0 = (unsigned char*)snd_buf + snd_buf_cur;
    snd_queue[snd_fill].f_4 = a2;
    if (snd_fill == 59)
        snd_fill = 0;
    else
        ++snd_fill;
    ++snd_cnt;
    snd_buf_cur += a2;
}

static void sndRelease(void)
{
    MemFree(&snd_mem_buf);
}

static void sndPause(void)
{
    if (snd_hDriver == -1)
        return;

    snd_paused = 1;
}

static void sndResume(void)
{
    if (snd_hDriver == -1)
        return;

    snd_paused = 0;
}
