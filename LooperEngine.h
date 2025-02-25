/*
  ==============================================================================

    LooperEngine.h
    Created: 16 Jun 2024 6:46:50pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "Track.h"
#include "CopyPaste.h"

#define TRACK_COUNT 4

class LooperEngine {
public:
    LooperEngine(MidiOut* midiOut) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].initialize(t, midiOut);
            track[t].setChannelOut(t + 1 + 4); //temp
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

    void toggleIsRecording() {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].toggleIsRecording();
        }
    }

    void resetTransport() {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].resetTransport();
        }
    }

    void toggleTrackMode() {
        byte current = track[0].getSettings()->trackMode; //should replace 0 by currentExclusiveTrack?
        current += 1;
        current = current % KTrackModeCount;
        setTrackMode((TrackMode)current);
    }

    void toggleMute() {
        track[currentExclusiveTrack].getSettings()->toggleMute();
    }

    void copyPaste() {
        clipboard.toggle(&track[currentExclusiveTrack]);
    }

    void setTrackMode(TrackMode trackMode) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setTrackMode(trackMode);
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
        currentExclusiveTrack = trackIndex;
    }

    byte getCurrentExclusiveTrack() {
        return currentExclusiveTrack;
    }

    void setGlobalBarCount(byte newBarCount) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setBarCount(newBarCount);
        }
    }
    
    void setGlobalStepCount(byte newStepCount) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setStepCount(newStepCount);
        }
    }
    
    void clearAll() {
        track[currentExclusiveTrack].clearAllSeq();
    }

    void clearAllTracks() {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].clearAllSeq();
        }
    }

    void fill() {
        track[currentExclusiveTrack].fill();
    }

    void setEraserState(bool state) {
        track[currentExclusiveTrack].setEraserState(state);
    }

    void setModulationWheel(byte value) {
        track[currentExclusiveTrack].setModulationWheel(value);
    }

    Transport* getTransport() {
        return track[currentExclusiveTrack].getTransport();
    }

    TrackSettings* getSettings() {
        return track[currentExclusiveTrack].getSettings();
    }

    TrackSettings* getTrackSettings(byte trackIndex) {
        return track[trackIndex].getSettings();
    }
    
private:
    byte currentExclusiveTrack = 0;
    Track track[TRACK_COUNT];
    byte noteChannel = 1;
    CopyPaste clipboard;
};
