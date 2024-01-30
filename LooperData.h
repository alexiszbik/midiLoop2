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
        selectedChannel = value;
      }
      break;

      _allTrackCase(barCount) :  {
        byte trackId = getTrackId(p, barCount1);
        seq.tracks[trackId].barCount = value;
      }
      break;
    }
  }
  
public:
  byte selectedChannel = 1;
  Sequencer seq;
  bool isPlaying = false;
  bool isRecording = true;
};