#pragma once

#include "DisplayManager.h"
#include "LooperData.h"

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

  void setFromCC(byte addrFromCC, byte ccValue) {
    parameters.setFromCC(addrFromCC, ccValue);
  }

  void addNote(byte note) {
    if (data.isPlaying) {
      data.seq.addNote(note);
    }
  }

  void play(bool state) {
    if (state) {
      data.seq.resetTicks();
    }
    data.play(state);
  }

  void tick() {
    data.seq.tick();
  }

  void updateFeedback() {
    displayManager.update();
  }

private:
  Parameters parameters = Parameters(this);  
  DisplayManager displayManager;

  LooperData data;
  
};