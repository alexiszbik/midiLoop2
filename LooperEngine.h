/*
  ==============================================================================

    LooperEngine.h
    Created: 16 Jun 2024 6:46:50pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "Track.h"
#include "Tempo.h"
#include "CopyPaste.h"

#define TRACK_COUNT 4

class LooperEngine : public TransportDelegate {
public:
    LooperEngine(MidiOut* midiOut) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].initialize(t, midiOut, &transport);
            track[t].setChannelOut(t + 1 + 4); //temp
        }
        track[0].setIsSelected(true);
    }
public:
    void tick() {
        tempo.tick();
        transport.tick(getTempo());
    }

    int getTempo() {
        return tempo.getTempo();
    }
    
    void setIsPlaying(bool isPlaying) {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].setIsPlaying(isPlaying);
        }
    }

    void setGroove(int groove) {
        transport.setGroove(groove);
    }
    
    void setIsRecording(bool isRecording) {
        transport.setRecordingState(isRecording);
    }

    void toggleIsRecording() {
        transport.toggleRecording();
    }

    void resetTransport() {
        transport.willReset();
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

    void copy(byte channel) {
        channel = fmax(fmin(channel, 3), 0);
        clipboard.copy(&track[channel]);
    }

    void paste(byte channel) {
        channel = fmax(fmin(channel, 3), 0);
        clipboard.paste(&track[channel]);
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
        transport.barCount = newBarCount;
    }
    
    void setGlobalStepCount(byte newStepCount) {
        transport.stepsPerBar = newStepCount;
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
        return &transport;
    }

    TrackSettings* getSettings() {
        return track[currentExclusiveTrack].getSettings();
    }

    TrackSettings* getTrackSettings(byte trackIndex) {
        return track[trackIndex].getSettings();
    }

    void loop() {
        transport.loop();
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].processNotesOn();
        }
    }

    virtual void didChangeStep (int newStep) override {
        for (byte t = 0; t < TRACK_COUNT; t++) {
            track[t].didChangeStep(newStep);
        }
    }
    
private:
    Tempo tempo;
    byte currentExclusiveTrack = 0;
    Track track[TRACK_COUNT];
    byte noteChannel = 1;
    CopyPaste clipboard;
    Transport transport = Transport(this);
    
};
