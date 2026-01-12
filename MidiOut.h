/*
 ==============================================================================
 
 MidiOutput.h
 Created: 16 Jun 2024 9:22:21pm
 Author:  Alexis ZBIK
 
 ==============================================================================
 */

#pragma once

#define MAX_VELOCITY 127

class MidiOut {
public:
    virtual ~MidiOut() {}
public:
    virtual void sendNote(byte trackIndex, byte channel, byte pitch, byte velocity) {
        digitalWrite(LED_TRACK_PLAY+trackIndex, velocity ? HIGH : LOW);
        MIDI.sendNoteOn(pitch, velocity, channel);
    }

    virtual void sendModulationWheel(byte channel, byte value) {
        MIDI.sendControlChange(1, value, channel);
    }
};
