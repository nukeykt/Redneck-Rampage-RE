#include <stdlib.h>
#include "sosm.h"

VOID sosMIDICode15Start(VOID) {}


W32 sosMIDIPauseSong(HANDLE a1, W32 a2)
{
    W32 v4;
    W32 v8;
    PSONG vc;
    if (a1 >= _MIDI_MAX_SONGS)
        return _ERR_INVALID_HANDLE;
    vc = _sMIDISystem.pSongList[a1];
    if (!vc)
        return _ERR_INVALID_HANDLE;

    if (vc->wFlags & _PAUSED)
        return _ERR_PAUSED;
    vc->wFlags |= _PAUSED;
    if (a2 & _MUTE)
        sosMIDIMuteSong(a1);
    return _ERR_NO_ERROR;
}

W32 sosMIDIResumeSong(HANDLE a1)
{
    PSONG vc;
    if (a1 >= _MIDI_MAX_SONGS)
        return _ERR_INVALID_HANDLE;
    vc = _sMIDISystem.pSongList[a1];
    if (!vc)
        return _ERR_INVALID_HANDLE;

    if (!(vc->wFlags & _PAUSED))
        return _ERR_NOT_PAUSED;
    vc->wFlags &= ~_PAUSED;
    if (vc->wFlags & _MUTE)
        sosMIDIUnMuteSong(a1);
    return _ERR_NO_ERROR;
}

VOID sosMIDICode15End(VOID) {}

