#ifndef _MVELIB32_H_
#define _MVELIB32_H_

typedef void* cdecl (*mve_cb_alloc)(unsigned int a1);
typedef void cdecl (*mve_cb_free)(void *a1);
typedef unsigned int cdecl (*mve_cb_read)(int a1, void *a2, unsigned int a3);
typedef void cdecl (*mve_cb_SoundHook)(unsigned char *a1, unsigned int a2, unsigned int a3, unsigned int a4);
typedef void cdecl (*mve_cb_ShowFrame)(unsigned char *a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, unsigned int a9);
typedef void cdecl (*mve_cb_SetPalette)(unsigned char *a1, unsigned int a2, unsigned int a3);
typedef int cdecl (*mve_cb_ctl)(void);

void cdecl MVE_memCallbacks(mve_cb_alloc a1, mve_cb_free a2);
void cdecl MVE_logDumpStats(void);
void cdecl MVE_ioCallbacks(mve_cb_read a1);
void cdecl MVE_memIO(void* a1, unsigned int a2);
void cdecl MVE_AIL_sndInit(void);
void cdecl MVE_SOS_sndInit(int a1);
unsigned int cdecl MVE_sndVolume(unsigned int a1);
void cdecl MVE_rmemSND(void);

#endif // !_MVELIB32_H_
