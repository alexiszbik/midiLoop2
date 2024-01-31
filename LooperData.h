#pragma once

#include "Sequencer.h"
#include "Parameters.h"

class LooperData {
public:
  byte getTrackId(Param p, Param firstTrackP) {
    return p - firstTrackP;
  }

  void parameterChanged(Param p, byte value)  {
    switch(p) {
      case channelSelect: {
        seq.selectedChannel = value;
      }
      break;

      _allTrackCase(barCount) :  {
        byte trackId = getTrackId(p, barCount1);
        seq.tracks[trackId].barCount = value;
      }
      break;

      _allTrackCase(stepCount) :  {
        byte trackId = getTrackId(p, stepCount1);
        seq.tracks[trackId].stepsPerBar = value;
      }
      break;
    }
  }

  void play(bool state) {
    isPlaying = state;
    if (!state) {
      seq.triggerNoteOff();
    }
    
  }
  
public:
  Sequencer seq;
  bool isPlaying = false;
  bool isRecording = true;

};