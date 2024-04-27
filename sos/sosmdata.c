#include "sosm.h"

W32 _wCData1Start = 0;
_MIDI_SYSTEM  _sMIDISystem = { 0 };
W32 ( cdecl far * _lpMIDICBCKFunctions[_MIDI_DRV_FUNCTIONS] )( LPSTR, W32, W32 ) = { 0 };
W32 ( cdecl far * _lpMIDIDIGIFunctions[_MIDI_DRV_FUNCTIONS] )( LPSTR, W32, W32 ) = { 0 };
W32 ( cdecl far * _lpMIDIWAVEFunctions[_MIDI_DRV_FUNCTIONS] )( LPSTR, W32, W32 ) = { 0 };
W32 _wMIDIDriverTotalChannels[_MIDI_DEFINED_DRIVERS] = { 0 };
W32 _wMIDIDriverChannel[ _MIDI_DEFINED_DRIVERS ][ _MIDI_MAX_CHANNELS ] = { 0 };
_MIDI_DRIVER  _sMIDIDriver[_MIDI_MAX_DRIVERS] = { 0 };
VOID ( cdecl far * _lpMIDICBCKFunction )( LPSTR, W32, W32 ) = 0;
_MIDI_DIGI_CHANNEL _sMIDIDIGIChannel[8] = { 0 };
_MIDI_DIGI_SYSTEM _sSOSMIDIDIGIDriver[8];
_MIDI_DIGI_QUEUE _sSOSMIDIDIGIQueue[1]; 
W32 _wCData1End = 0;