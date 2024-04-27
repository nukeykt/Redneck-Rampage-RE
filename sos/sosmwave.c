#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <i86.h>
#include "sosm.h"

W32 cdecl far waveSendData(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far waveInit(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far waveUnInit(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far waveReset(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

W32 cdecl far waveSetInstrumentData(LPSTR a1, W32 a2, W32 a3)
{
    return 0;
}

VOID cdecl far waveSampleCallback(W32 a1, W32 a2, W32 a3)
{
}

W32 cdecl sosWAVEPStopSample()
{
    return 0;
}

W32 cdecl waveChannelResetControllers()
{
    return 0;
}

W32 cdecl waveChannelSetVolume()
{
    return 0;
}

W32 cdecl waveChannelSetPan()
{
    return 0;
}

W32 cdecl waveChannelSetBend()
{
    return 0;
}

W32 cdecl waveChannelNotesOff()
{
    return 0;
}

void StringOut(BYTE a1, BYTE a2, BYTE *a3, BYTE a4)
{
    LPBYTE v18;

    v18 = MK_FP(xgetES(), 0xb8000);

    v18 += a1 * 2 + a2 * 160;

    while (*a3)
    {
        *v18++ = *a3++;
        *v18++ = a4;
    }
}

W32 cdecl waveCalculatePitchBend()
{
    return 0;
}

W32 _wSOSWAVEInsDataSet = 0;

W32 dwwavepitchtable[] = {
0x200, 0x21e, 0x23e, 0x260, 0x285, 0x2ab, 0x2d4, 0x2ff, 0x32c,
0x35d, 0x390, 0x3c6, 0x400, 0x43d, 0x47d, 0x4c1, 0x50a, 0x556,
0x5a8, 0x5fe, 0x659, 0x6ba, 0x720, 0x78d, 0x800, 0x87a, 0x8fb,
0x983, 0x0a14, 0x0aad, 0x0b50, 0x0bfc, 0x0cb3, 0x0d74, 0x0e41,
0x0f1a, 0x1000, 0x10f4, 0x11f6, 0x1307, 0x1428, 0x155b, 0x16a1,
0x17f9, 0x1966, 0x1ae9, 0x1c83, 0x1e35, 0x2000, 0x21e8, 0x23ed,
0x260e, 0x2851, 0x2ab7, 0x2d42, 0x2ff3, 0x32cd, 0x35d2, 0x3907,
0x3c6a, 0x4000, 0x43d1, 0x47da, 0x4c1c, 0x50a2, 0x556e, 0x5a85,
0x5fe6, 0x659a, 0x6ba5, 0x720e, 0x78d4, 0x8000, 0x87a2, 0x8fb4,
0x9838, 0x0a145, 0x0aadc, 0x0b50a, 0x0bfcd, 0x0cb34, 0x0d74a,
0x0e41c, 0x0f1a9, 0x10000, 0x10f44, 0x11f69, 0x13070, 0x1428b,
0x155b9, 0x16a14, 0x17f9b, 0x19668, 0x1ae94, 0x1c838, 0x1e353,
0x20000, 0x21e88, 0x23ed2, 0x260e0, 0x28516, 0x2ab72, 0x2d428,
0x2ff36, 0x32cd0, 0x35d28, 0x39070, 0x3c6a6, 0x40000, 0x43d10,
0x47da4, 0x4c1c0, 0x50a2c, 0x556e4, 0x5a850, 0x5fe6c, 0x659a0,
0x6ba50, 0x720e0, 0x78d4c, 0x80000, 0x87a20, 0x8fb48, 0x98380,
0x0a1458, 0x0aadc8, 0x0b50a0, 0x0bfcd8, 0x0cb340, 0x0d74a0,
0x0e41c0, 0x0f1a98, 0x100000, 0x10f440, 0x11f690, 0x130700,
0x1428b0, 0x155b90, 0x16a140, 0x17f9b0, 0x196680, 0x1ae940,
0x1c8380, 0x1e3530, 0x200000, 0x21e880, 0x23ed20, 0x260e00,
0x285160, 0x2ab720, 0x2d4280, 0x2ff360, 0x32cd00, 0x35d280,
0x390700, 0x3c6a60, 0x400000, 0x43d100, 0x47da40, 0x4c1c00,
0x50a2c0, 0x556e40, 0x5a8500, 0x5fe6c0, 0x659a00, 0x6ba500,
0x720e00, 0x78d4c0, 0x800000, 0x87a200, 0x8fb480, 0x983800,
0x0a14580, 0x0aadc80, 0x0b50a00, 0x0bfcd80, 0x0cb3400, 0x0d74a00,
0x0e41c00, 0x0f1a980

};

BYTE waveVoice[][20] = {
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 },
    { 0, 0xff, 0, 0, 0, 0, 1, 0, 0xff, 0, 0, 0, 0xff, 0, 0, 0, 0xff, 0 }
};

BYTE waveChannel[][6] = {
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
    { 0, 0x7f, 0x40, 2, 0, 0x80 },
};

BYTE waveDIGIMIDIHandle[20] = { 0 };