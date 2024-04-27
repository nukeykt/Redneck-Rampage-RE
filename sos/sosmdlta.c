#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode6Start(VOID) {}

W32 sosMIDIGetDeltaTime(PSTR a1, DWORD *a2)
{
    BYTE v4;
    DWORD v8;
    DWORD vc;
    DWORD v10;
    DWORD v14;

    v8 = 0;
    vc = 0;
    v10 = 0;
    v14 = 0;

    do
    {
        v10 <<= 7;
        v14++;
        v4 = *a1++;
        if (!(v4 & 0x80))
            v8 = 1;
        v4 &= 0x7f;
        v10 |= v4;
    } while (!v8);
    *a2 = v10;
    return v14;
}

VOID sosMIDICode6End(VOID) {}

