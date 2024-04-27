#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode22Start(VOID) {}

W32 sosMIDISetSongVolume(HANDLE a1, W32 a2)
{
    DWORD v4;
    DWORD v8;
    DWORD vc;
    PSONG v10;
    PTRACK v14;
    BYTE *v18;
    PCHANNEL v1c;
    DWORD v20;
    DWORD v24;
    DWORD *v28;
    DWORD v2c;

    if (a1 >= 32)
        return 10;

    v10 = _sMIDISystem.pSongList[a1];

    if (!v10)
        return 10;

    v10->sVolume.wVolume = a2;
    if (v10->wFlags & 1)
        return 0;

    v18 = v10->pChannelTrackDependencyLists;
    v4 = v18[0];
    ++v18;
    for (vc = 0; vc < v4; vc++)
    {
        v8 = v18[0];
        ++v18;
        v28 = v18;
        for (v2c = 0; v2c < v8; v2c++)
        {
            v14 = v28[0];
            if ((v14->wFlags & 0x10) == 0)
            {
                v1c = v14->pDriverChannel;
                v24 = v1c->wVolume * a2 / 127;
                v24 = v24 * _sMIDISystem.wVolume / 127;
                v24 = v24 * v1c->wDriverVolume / 127;
                v1c->wVolumeScalar = v24;
                if (v1c->wChannel != -1)
                {
                    sosMIDISendMIDIEvent(v14->hDriver, 0xb0 | v1c->wChannel, 7, v24, 3);
                }
                break;
            }

            v28++;
        }
        v18 += v8 * 4;
    }
    return 0;
}

W32 sosMIDIFadeSong(HANDLE a1, W32 a2, W32 a3, W32 a4, W32 a5, W32 a6)
{
    DWORD v4;
    DWORD v8;
    DWORD vc;
    DWORD v10;
    DWORD v14;
    PSONG v18;

    if (a1 >= 32)
        return 10;

    v18 = _sMIDISystem.pSongList[a1];
    if (!v18)
        return 10;

    if (v18->hEvent == -1)
        return 10;
    if (a2 & 1)
        v4 = a5 - a4;
    else
        v4 = a4 - a5;

    v8 = sosTIMERGetEventRate(v18->hEvent);
    v10 = 6553600 / v8;
    vc = (a3 << 16) / v10;
    if (vc == 0)
    {
        if (a2 & 4)
        {
            sosMIDIStopSong(a1);
            return 0;
        }
        else
        {
            sosMIDISetSongVolume(a1, a5);
            return 0;
        }
    }

    sosMIDISetSongVolume(a1, a4);

    v14 = (v4 << 16) / vc;

    v18->sVolume.wDirection = a2;
    v18->sVolume.dwFraction = v14;
    v18->sVolume.dwFadeVolume = a4 << 16;
    v18->sVolume.wTicks = vc;
    v18->sVolume.wStart = a4;
    v18->sVolume.wStartVolume = a4;
    v18->sVolume.wEnd = a5;
    v18->sVolume.wSkip = a6;
    v18->sVolume.wSkipCurrent = a6;
    v18->sVolume.wFlags |= 0x8008;
    return 0;
}

W32 sosMIDISetMasterVolume(W32 a1)
{
    DWORD v4;
    PSONG v8;

    v4 = 0;

    _sMIDISystem.wVolume = a1;
    for (v4 = 0; v4 < 32; v4++)
    {
        if (_sMIDISystem.pSongList[v4])
        {
            v8 = _sMIDISystem.pSongList[v4];
            sosMIDISetSongVolume(v4, v8->sVolume.wVolume);
        }
    }
    return 0;
}

W32 sosMIDISetDriverVolume(HANDLE a1, W32 a2)
{
    DWORD v4;
    BYTE *v8;
    DWORD vc;
    if (a1 >= 8)
        return 10;

    v8 = _sMIDIDriver[a1].pChannel;

    vc = v8[0];
    ++v8;
    for (v4 = 0; v4 < vc; v4++)
    {
        ((PCHANNEL)v8)->wDriverVolume = a2;
        v8 += 0x24;
    }
    sosMIDISetMasterVolume(_sMIDISystem.wVolume);

    return 0;
}

VOID sosMIDICode22End(VOID) {}
