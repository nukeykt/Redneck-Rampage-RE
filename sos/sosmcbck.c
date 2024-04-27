#include "sosm.h"

VOID sosMIDICode2Start(VOID) {}

W32 cdecl far sosCBCKSendData(LPSTR a1, W32 a2, W32 a3)
{
    if (_lpMIDICBCKFunction)
        _lpMIDICBCKFunction(a1, a2, a3);
    return 0;
}

W32 cdecl far sosCBCKInit(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far sosCBCKUnInit(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far sosCBCKReset(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far sosCBCKSetInstrumentData(LPSTR a1, W32 a2, W32 a3)
{
    _lpMIDICBCKFunction = (void far *)a1;
    return 0;
}

VOID sosMIDICode2End(VOID) {}
