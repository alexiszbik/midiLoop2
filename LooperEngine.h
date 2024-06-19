/*
  ==============================================================================

    LooperEngine.h
    Created: 16 Jun 2024 6:46:50pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "Track.h"

#define TRACK_COUNT 4

class LooperEngine {
public:
    LooperEngine(MidiOut* midiOut) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].initialize(midiOut);
            track[t].setChannelOut(t + 5); //temp
        }
        track[0].setIsSelected(true);
    }
public:
    void tick() {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].tick();
        }
    }
    
    void setIsPlaying(bool isPlaying) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setIsPlaying(isPlaying);
        }
    }
    
    void setIsRecording(bool isRecording) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setIsRecording(isRecording);
        }
    }

    void setUseArp(bool useArp) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setUseArp(useArp);
        }
    }
    
    void playNoteOn(byte note, byte velocity) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].playNote(note, velocity);
        }
    }
    
    void selectExclusiveTrack(byte trackIndex) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setIsSelected(trackIndex == t);
        }
    }
    
    void clearAll() {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].clearAllSeq();
        }
    }

    Transport* getTransport() {
        return track[0].getTransport();
    }

    TrackSettings* getSettings() {
        return track[0].getSettings();
    }
    
private:
    Track track[TRACK_COUNT];
    
    byte noteChannel = 1;
    
};
