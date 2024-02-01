#pragma once

#include "Sequencer.h"
#include "Parameters.h"

#define APPLY_PARAM(paramName, varName) \
_allTrackCase(paramName) :  { \
  byte trackId = getTrackId(p, paramName##1); \
  seq.tracks[trackId].varName = value; \
} \
  break; \

class LooperData {
public:
  void parameterChanged(Param p, byte value)  {
    switch(p) {
      case channelSelect: {
        seq.selectedChannel = value;
      }
      break;

      APPLY_PARAM(barCount, barCount)

      APPLY_PARAM(stepCount, stepsPerBar)

      APPLY_PARAM(arpState, arpState)
    }
  }

  void play(bool state) {
    isPlaying = state;
    if (!state) {
      seq.triggerNoteOff();
    }
  }

  void tick() {
    if (isPlaying) {
     seq.tick();
    }
  }
  
public:
  Sequencer seq;
  bool isPlaying = false;
  bool isRecording = false;

};