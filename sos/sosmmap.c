#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode13Start(VOID) {}

BOOL sosMIDIMapTrack(PTRACK a1)
{
    WORD *v4;
    DWORD v8;
    DWORD vc;
    DWORD v10;

    v4 = (WORD*)(a1+1);
    v8 = 0;
    v10 = 0;
    while (*v4 != 0 && v8 < 8 && !v10)
    {
        for (vc = 0; vc < 8; vc++)
        {
            if (*v4 == _MIDI_SOUND_MASTER_II)
            {
                switch (_sMIDIDriver[vc].wID)
                {
                    case _MIDI_SOUND_MASTER_II:
                    case _MIDI_MPU_401:
                    case _MIDI_AWE32:
                        v10 = 1;
                        break;
                }
            }
            else if (*v4 == _MIDI_FM)
            {
                switch (_sMIDIDriver[vc].wID)
                {
                    case _MIDI_FM:
                    case _MIDI_OPL3:
                        v10 = 1;
                        break;
                }
            }
            else
            {
                if (*v4 == _sMIDIDriver[vc].wID)
                    v10 = 1;
            }
            if (v10)
                break;
        }

        if (v10)
            break;
        v4++;
    }
    if (!v10)
        return 1;
    a1->hDriver = vc;
    return 0;
}

VOID sosMIDICode13End(VOID) {}
