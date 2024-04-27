#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode17Start(VOID) {}

W32 sosMIDISendMIDIData(HANDLE a1, LPSTR a2, DWORD a3)
{
    _sMIDIDriver[a1].lpfnFunction[_MIDI_DRV_SEND_DATA](a2, a3, a1);
    return 0;
}

W32 sosMIDISendMIDIEvent(HANDLE a1, W32 a2, W32 a3, W32 a4, W32 a5)
{
    char data[8];
    data[0] = a2;
    data[1] = a3;
    data[2] = a4;
    _sMIDIDriver[a1].lpfnDataFunction(data, a5, a1);
    return 0;
}

VOID sosMIDICode17End(VOID) {}
