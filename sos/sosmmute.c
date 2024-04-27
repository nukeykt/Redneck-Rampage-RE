#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode14Start(VOID) {}

W32 sosMIDIMuteSong(HANDLE a1)
{
    int v4;
    PSONG v8;
    if (a1 >= _MIDI_MAX_SONGS)
        return _ERR_INVALID_HANDLE;
    v8 = _sMIDISystem.pSongList[a1];
    if (!v8)
        return _ERR_INVALID_HANDLE;

    v4 = v8->sVolume.wVolume;
    sosMIDISetSongVolume(a1, 0);
    v8->sVolume.wVolume = v4;
    v8->wFlags |= _MUTE;
    return 0;
}

W32 sosMIDIUnMuteSong(HANDLE a1)
{
    int v4;
    PSONG v8;
    if (a1 >= _MIDI_MAX_SONGS)
        return _ERR_INVALID_HANDLE;
    v8 = _sMIDISystem.pSongList[a1];
    if (!v8)
        return _ERR_INVALID_HANDLE;

    v8->wFlags &= ~_MUTE;
    sosMIDISetSongVolume(a1, v8->sVolume.wVolume);
    return 0;
}

VOID sosMIDICode14End(VOID) {}
