/*
  ==============================================================================

    TrackSettings.h
    Created: 16 Jun 2024 7:01:39pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#define STEP_RESOLUTION 6

#include "Bounded.h"

class StepsPerBar : public Bounded<byte> {
public:
    static const byte Min = 1;
    static const byte Max = 16;
public:
    StepsPerBar(byte value) : Bounded(value, Min, Max) {}
};

class BarCount : public Bounded<byte> {
public:
    static const byte Min = 1;
    static const byte Max = 8;
public:
    BarCount(byte value) : Bounded(value, Min, Max) {}
};

class TrackSettings {
public:
    int getSeqLength() {
        return getStepCount() * STEP_RESOLUTION;
    }
    
    int getStepCount() {
        return stepsPerBar * barCount;
    }
    
    byte getStepResolution() {
        return stepResolution;
    }

public:
    StepsPerBar stepsPerBar = 16;
    BarCount barCount = 1;
    byte channelOut = 1;
    bool isSelected = false;
    bool isRecording = false;
    bool arpIsOn = false;
    
private:
    byte stepResolution = 6; // 24/4 -> 1 sixteenth
};
