/*
  ==============================================================================

    MidiOutput.h
    Created: 16 Jun 2024 9:22:21pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

//only for test
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
#else
#define _SIMULATOR_
#endif

#if defined(_SIMULATOR_)
//#include <iostream>
#endif

#define MAX_VELOCITY 127

class MidiOut {
public:
    void sendNote(byte channel, byte pitch, byte velocity) {
      #if defined(_SIMULATOR_)
        //std::cout << channel << " " << pitch << " " << velocity << std::endl;
      #endif
      MIDI.sendNoteOn(pitch, velocity, channel);
    }
};
