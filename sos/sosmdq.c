#include "sosm.h"

VOID sosMIDICode7Start(VOID) {}

HSOS digiQueueInit(WORD a1)
{
    WORD v4;
    WORD v8;

    v4 = 0;
    while (v4 < 1)
    {
        if ((_sSOSMIDIDIGIQueue[v4].wFlags & 0x8000) == 0)
            break;
        v4++;
    }

    if (v4 == 1)
        return 0xffff;
    _sSOSMIDIDIGIQueue[v4].wFlags |= 0x8000;

    for (v8 = 0; v8 < 32; v8++)
    {
        _sSOSMIDIDIGIQueue[v4].sElement[v8].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[v4].sElement[v8].wID = 0xffff;
        _sSOSMIDIDIGIQueue[v4].sElement[v8].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[v4].sElement[v8].wVelocity = 0xffff;
    }
    _sSOSMIDIDIGIQueue[v4].wHead = 0;
    _sSOSMIDIDIGIQueue[v4].wTail = 0;
    _sSOSMIDIDIGIQueue[v4].wUsedVoices = 0;
    _sSOSMIDIDIGIQueue[v4].wMaxVoices = a1;

    return v4;
}

VOID digiQueueUnInit(HSOS a1)
{
    _sSOSMIDIDIGIQueue[a1].wFlags &= ~0x8000;
}

WORD digiQueueAddItem(HSOS a1, HSOS a2, WORD a3, WORD a4, WORD a5)
{
    WORD v4;

    _SOS_DIGI_QUEUE_ELEMENT *v1c;

    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices >= _sSOSMIDIDIGIQueue[a1].wMaxVoices)
        return 0xffff;

    v4 = _sSOSMIDIDIGIQueue[a1].wHead;

    v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];

    v1c->hSample = a2;
    v1c->wID = a3;
    v1c->wVelocity = a4;
    v1c->wChannel = a5;
    ++_sSOSMIDIDIGIQueue[a1].wUsedVoices;
    ++_sSOSMIDIDIGIQueue[a1].wHead;
    _sSOSMIDIDIGIQueue[a1].wHead &= 0x1f;
    return v4;
}

WORD digiQueueGetItem(HSOS a1, WORD a2)
{
    WORD v4;
    WORD v8;
    WORD vc;

    _SOS_DIGI_QUEUE_ELEMENT *v1c;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        for (v4 = 0; v4 < 32; v4++)
        {
            v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];
            if (v1c->wChannel == a2 && v1c->hSample != 0xffff)
                break;
        }
        if (v4 == 32)
            return 0xffff;

        vc = v1c->hSample;
        while (v4 != _sSOSMIDIDIGIQueue[a1].wTail)
        {
            v8 = (v4 - 1) & 31;

            _sSOSMIDIDIGIQueue[a1].sElement[v4] = _sSOSMIDIDIGIQueue[a1].sElement[v8];
            v4 = v8;
        }
        _sSOSMIDIDIGIQueue[a1].sElement[v4].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wID = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wVelocity = 0xffff;
        ++_sSOSMIDIDIGIQueue[a1].wTail;
        _sSOSMIDIDIGIQueue[a1].wTail &= 31;
        --_sSOSMIDIDIGIQueue[a1].wUsedVoices;
        return vc;
    }
    return 0xffff;
}

WORD digiQueueDeleteItemMIDI(WORD a1, WORD a2, WORD a3)
{
    WORD v4;
    WORD v8;
    WORD vc;

    _SOS_DIGI_QUEUE_ELEMENT *v1c;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        for (v4 = 0; v4 < 32; v4++)
        {
            v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];
            if (v1c->wID == a2 && v1c->wChannel == a3 && v1c->hSample != 0xffff)
                break;
        }
        if (v4 == 32)
            return 0xffff;

        vc = v1c->hSample;
        while (v4 != _sSOSMIDIDIGIQueue[a1].wTail)
        {
            v8 = (v4 - 1) & 31;

            _sSOSMIDIDIGIQueue[a1].sElement[v4] = _sSOSMIDIDIGIQueue[a1].sElement[v8];
            v4 = v8;
        }
        _sSOSMIDIDIGIQueue[a1].sElement[v4].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wID = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wVelocity = 0xffff;
        ++_sSOSMIDIDIGIQueue[a1].wTail;
        _sSOSMIDIDIGIQueue[a1].wTail &= 31;
        --_sSOSMIDIDIGIQueue[a1].wUsedVoices;
        return vc;
    }
    return 0xffff;
}

WORD digiQueueDeleteItem(HSOS a1, HSOS a2)
{
    WORD v4;
    WORD v8;

    _SOS_DIGI_QUEUE_ELEMENT* v1c;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        for (v4 = 0; v4 < 32; v4++)
        {
            v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];
            if (v1c->hSample == a2)
                break;
        }
        if (v4 == 32)
            return 0xffff;

        while (v4 != _sSOSMIDIDIGIQueue[a1].wTail)
        {
            v8 = (v4 - 1) & 31;

            _sSOSMIDIDIGIQueue[a1].sElement[v4] = _sSOSMIDIDIGIQueue[a1].sElement[v8];
            v4 = v8;
        }
        _sSOSMIDIDIGIQueue[a1].sElement[v4].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wID = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wVelocity = 0xffff;
        ++_sSOSMIDIDIGIQueue[a1].wTail;
        _sSOSMIDIDIGIQueue[a1].wTail &= 31;
        --_sSOSMIDIDIGIQueue[a1].wUsedVoices;
        return a2;
    }
    return 0xffff;
}

WORD digiQueueFindItemMIDI(HSOS a1, WORD a2, WORD a3)
{
    WORD v4;
    WORD v8;
    WORD vc;

    _SOS_DIGI_QUEUE_ELEMENT *v1c;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        for (v4 = 0; v4 < 32; v4++)
        {
            v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];
            if (v1c->wID == a2 && v1c->wChannel == a3 && v1c->hSample != 0xffff)
                break;
        }
        if (v4 == 32)
            return 0xffff;
        vc = v1c->hSample;
        return vc;
    }
    return 0xffff;
}

WORD digiQueueGetItemWAVE(HSOS a1)
{
    WORD v4;
    WORD v8;
    WORD vc;

    _SOS_DIGI_QUEUE_ELEMENT *v1c;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        for (v4 = 0; v4 < 32; v4++)
        {
            v1c = &_sSOSMIDIDIGIQueue[a1].sElement[v4];
            if (v1c->hSample != 0xffff)
                break;
        }
        if (v4 == 32)
            return 0xffff;
        vc = v1c->hSample;

        while (v4 != _sSOSMIDIDIGIQueue[a1].wTail)
        {
            v8 = (v4 - 1) & 31;

            _sSOSMIDIDIGIQueue[a1].sElement[v4] = _sSOSMIDIDIGIQueue[a1].sElement[v8];
            v4 = v8;
        }
        _sSOSMIDIDIGIQueue[a1].sElement[v4].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wID = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[v4].wVelocity = 0xffff;
        ++_sSOSMIDIDIGIQueue[a1].wTail;
        _sSOSMIDIDIGIQueue[a1].wTail &= 31;
        --_sSOSMIDIDIGIQueue[a1].wUsedVoices;
        return vc;
    }
    return 0xffff;
}

WORD digiQueueDeleteItemWAVE(HSOS a1, WORD a2)
{
    WORD v4;
    if (_sSOSMIDIDIGIQueue[a1].wUsedVoices)
    {
        while (a2 != _sSOSMIDIDIGIQueue[a1].wTail)
        {
            v4 = (a2 - 1) & 31;

            _sSOSMIDIDIGIQueue[a1].sElement[a2] = _sSOSMIDIDIGIQueue[a1].sElement[v4];
            a2 = v4;
        }
        _sSOSMIDIDIGIQueue[a1].sElement[a2].hSample = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[a2].wID = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[a2].wChannel = 0xffff;
        _sSOSMIDIDIGIQueue[a1].sElement[a2].wVelocity = 0xffff;
        ++_sSOSMIDIDIGIQueue[a1].wTail;
        _sSOSMIDIDIGIQueue[a1].wTail &= 31;
        --_sSOSMIDIDIGIQueue[a1].wUsedVoices;
        return 0xffff;
    }
    return 0xffff;
}

VOID sosMIDICode7End(VOID) {}
