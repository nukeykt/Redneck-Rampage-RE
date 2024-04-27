#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <dos.h>
#include <i86.h>
#include <string.h>
#include "sosm.h"


VOID sosMIDICode19Start(VOID) {}

W32 sosMIDIInitSystem(PSTR a1, W32 a2)
{
    DWORD v4;
    DWORD v8;
    DWORD vc;
    if (_sMIDISystem.wFlags & 0x1000)
        return 24;

    v4 = sosMIDILockLibrary();
    if (v4)
        return v4;

    if (a1)
        strcpy(_sMIDISystem.szPath, a1);
    else
        _sMIDISystem.szPath[0] = 0;

    _sMIDISystem.wFlags = 0;
    _sMIDISystem.wTrackBranched = 0;
    _sMIDISystem.pStealList = 0;
    _sMIDISystem.pSongFirst = 0;
    _sMIDISystem.wMT32SendData = 0;
    _sMIDISystem.wVolume = 127;

    for (v8 = 0; v8 < 32; v8++)
    {
        _sMIDISystem.pSongList[v8] = 0;
    }

    for (v8 = 0; v8 < 8; v8++)
    {
        _sMIDIDriver[v8].wFlags = 0;
        _sMIDIDriver[v8].wID = 0;
        _sMIDIDriver[v8].pChannel = 0;
        for (vc = 0; vc < 30; vc++)
        {
            _sMIDIDriver[v8].lpfnFunction[vc] = 0;
        }
    }

    _lpMIDICBCKFunctions[0] = sosCBCKSendData;
    _lpMIDICBCKFunctions[1] = sosCBCKInit;
    _lpMIDICBCKFunctions[2] = sosCBCKUnInit;
    _lpMIDICBCKFunctions[3] = sosCBCKReset;
    _lpMIDICBCKFunctions[4] = sosCBCKSetInstrumentData;
    _lpMIDIDIGIFunctions[0] = sosDIGISendData;
    _lpMIDIDIGIFunctions[1] = sosDIGIInit;
    _lpMIDIDIGIFunctions[2] = sosDIGIUnInit;
    _lpMIDIDIGIFunctions[3] = sosDIGIReset;
    _lpMIDIDIGIFunctions[4] = sosDIGISetInstrumentData;
    _lpMIDIWAVEFunctions[0] = waveSendData;
    _lpMIDIWAVEFunctions[1] = waveInit;
    _lpMIDIWAVEFunctions[2] = waveUnInit;
    _lpMIDIWAVEFunctions[3] = waveReset;
    _lpMIDIWAVEFunctions[4] = waveSetInstrumentData;

    _sMIDISystem.wFlags |= 0x1000;

    return 0;
}

W32 sosMIDIUnInitSystem(VOID)
{
    DWORD v4;
    if (!(_sMIDISystem.wFlags & 0x1000))
        return 25;
    v4 = sosMIDIUnLockLibrary();
    if (v4)
        return v4;

    _sMIDISystem.wFlags &= ~0x1000;
    return 0;
}

VOID sosMIDICode19End(VOID) {}
