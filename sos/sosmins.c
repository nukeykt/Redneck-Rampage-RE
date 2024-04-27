#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode10Start(VOID) {}

W32 sosMIDISetInsData(HANDLE a1, LPSTR a2, W32 a3)
{
    return _sMIDIDriver[a1].lpfnFunction[_MIDI_DRV_SET_INST_DATA](a2, a1, a3);
}

W32 sosMIDISetMT32InsData(HANDLE a1, LPSTR a2, W32 a3)
{
    DWORD v4;
    HANDLE v8;
    DWORD vc;
    W32 v10;
    if (a3)
    {
        for (v4 = 0; v4 < 0xffff; v4++)
        {
        }

        v10 = sosTIMERRegisterEvent(1500, sosMIDIMT32Timer, &v8);
        if (v10)
            return v10;
        for (v4 = 0; v4 < a3; v4++)
        {
            while (!_sMIDISystem.wMT32SendData)
            {
                vc = 1;
            }
            _sMIDISystem.wMT32SendData = 0;
            _sMIDIDriver[a1].lpfnFunction[_MIDI_DRV_SET_INST_DATA](a2 + v4, 1, a1);
        }
        sosTIMERRemoveEvent(v8);
    }
    return 0;
}

VOID sosMIDIMT32Timer(VOID)
{
    _sMIDISystem.wMT32SendData = 1;
}

VOID sosMIDICode10End(VOID) {}
