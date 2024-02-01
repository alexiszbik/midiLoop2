#pragma once

#include "DisplayManager.h"
#include "LooperData.h"
#include "Macros.h"

class LooperEngine : public ParametersDelegate {
public:
  LooperEngine() {
  }

  void init() {
    displayManager.init(&data);
    parameters.readAll();
  }

  virtual void parameterChanged(Param p, byte value) override {
    data.parameterChanged(p, value);
  }

  void controlChange(byte control, byte value) {
    switch(control) {
      case Record :
        data.isRecording = midiValueToBool(value);
        break;

      case ClearAll :
        data.seq.clearAll();
        break;

      _allTrackCase(ClearTrack) :
        data.seq.tracks[getTrackId(control, ClearTrack1)].clearAll();
        break;

      default :
        parameters.setFromCC(control, value);
        break;
    }
    
  }

  void noteOn(byte note, byte velocity) {
    if (data.isPlaying && data.isRecording) {
      if (velocity > 0) {
        data.seq.noteOn(note);
      } else {
        data.seq.noteOff(note);
      }
    } else {
      data.seq.noteThru(note, velocity);
    }
  }

  void noteOff(byte note, byte velocity) {
    if (data.isPlaying && data.isRecording) {
      data.seq.noteOff(note);
    } else {
      data.seq.noteThru(note, 0);
    }
  }

  void play(bool state) {
    if (state) {
      data.seq.resetTicks();
    }
    data.play(state);
  }

  void tick() {
    data.tick();
  }

  void updateFeedback() {
    displayManager.update();
  }

private:

  Parameters parameters = Parameters(this);  
  DisplayManager displayManager;

  LooperData data;
  
};