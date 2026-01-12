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

enum TrackMode : byte {
  kSequence = 0,
  kSequencePoly,
  kArp,
  kHoldPoly,

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

    bool modeIsPoly() {
      return trackMode == kSequencePoly || trackMode == kHoldPoly;
    }

    bool modeIsHold() {
      return trackMode == kHoldPoly;
    }

    void toggleMute() {
        isMuted = !isMuted;
    }

public:
    StepsPerBar stepsPerBar = 16;
    BarCount barCount = 8;
    byte channelOut = 1;
    bool isSelected = false;
    bool isRecording = false;
    bool isMuted = false;
    TrackMode trackMode = kSequence;
    
private:
    byte stepResolution = 6; // 24/4 -> 1 sixteenth
};
