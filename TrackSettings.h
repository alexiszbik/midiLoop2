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
    byte channelOut = 1;
    bool isSelected = false;
    bool isMuted = false;
    TrackMode trackMode = kSequence;
};
