/*
  ==============================================================================

    TrackSettings.h
    Created: 16 Jun 2024 7:01:39pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#define STEP_RESOLUTION 6

#include "BoundedSettings.h"

enum TrackMode {
  kSequence = 0,
  kArp,

  KTrackModeCount
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

    bool useArp() {
      return trackMode == kArp;
    }

public:
    StepsPerBar stepsPerBar = 16;
    BarCount barCount = 1;
    byte channelOut = 1;
    bool isSelected = false;
    bool isRecording = false;
    TrackMode trackMode = kSequence;
    
private:
    byte stepResolution = 6; // 24/4 -> 1 sixteenth
};
