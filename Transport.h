/*
  ==============================================================================

    Transport.h
    Created: 16 Jun 2024 6:51:33pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "TrackSettings.h"

class TransportDelegate {
public:
  virtual ~TransportDelegate(){};
  void virtual didChangeStep(int newStep) = 0;
};

class Transport {
public:
  Transport(TrackSettings* settings, TransportDelegate* delegate)
    : settings(settings), delegate(delegate) {
  }

public:
  void tick() {
    if (isPlaying) {
      if (currentTick % settings->getStepResolution() == 0) {
        if (toBeReseted) {
          reset();
          toBeReseted = false;
        } else {
          currentStep = (currentTick / settings->getStepResolution());
        }
        delegate->didChangeStep(currentStep);
      }
      currentTick++;
      currentTick = currentTick % getLength();
    }
  }

  void setIsPlaying(bool isPlaying) {
    if (this->isPlaying && !isPlaying) {
      reset();
    }
    this->isPlaying = isPlaying;
  }

  bool getIsPlaying() {
    return this->isPlaying;
  }

  int getRecStep(bool* playNote) {
    int stepCount = settings->getStepCount();
    double stepRatio = (double)currentTick / (double)settings->getStepResolution();
    double flooredStepRatio = floor(stepRatio);
    double ratioFrac = stepRatio - flooredStepRatio;
    bool quantizeDown = ratioFrac < 0.5;
    *playNote = quantizeDown && ratioFrac != 0.0;
    int step = quantizeDown ? flooredStepRatio : flooredStepRatio + 1;
    //Serial.println(currentTick);
    //Serial.println(quantizeDown);
    return (step + stepCount) % stepCount;
  }


  byte getCurrentStep() {
    return currentStep;
  }

  void willReset() {
    toBeReseted = true;
  }

  void setGroove(byte groove) {
    this->groove = groove;
  }

  byte getGroove() {
    return groove;
  }

  void loop() {
  }

private:
  int getLength() {
    return settings->getSeqLength();
  }

  void reset() {
    currentTick = 0;
    currentStep = 0;
  }

private:
  int currentTick = 0;
  byte currentStep = 0;
  bool toBeReseted = false;

  bool isPlaying = false;
  byte groove = 0;


  TrackSettings* settings;
  TransportDelegate* delegate;
};
