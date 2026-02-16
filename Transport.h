/*
  ==============================================================================

    Transport.h
    Created: 16 Jun 2024 6:51:33pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "TrackSettings.h"
#include "Tempo.h"

class TransportDelegate {
public:
  virtual ~TransportDelegate(){};
  void virtual didChangeStep(int newStep) = 0;
};

class Transport {
public:
  Transport(TransportDelegate* delegate)
    : delegate(delegate) {
  }

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

  void tick(int tempo) {

    double quarterLen = 60. / tempo;
    double sxLen = quarterLen * 0.25;
    double grooved = sxLen * (groove * 0.01);

    if (isPlaying) {
      if (currentTick % getStepResolution() == 0) {
        if (toBeReseted) {
          reset();
          toBeReseted = false;
        } else {
          currentStep = (currentTick / getStepResolution());
        }

        bool isOddStep = currentStep % 2 == 1;
        unsigned long offset = isOddStep ? grooved * 1000 : 0;
        newStepTime = now + offset;
        ready = true;
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
    int stepCount = getStepCount();
    double stepRatio = (double)currentTick / (double)getStepResolution();
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
    now = millis();
    if (now > newStepTime && ready) {
      delegate->didChangeStep(currentStep);
      ready = false;
    }
  }

  bool getRecordingState() {
    return isRecording;
  }

  bool setRecordingState(bool r) {
    isRecording = r;
  }

  void toggleRecording() {
    isRecording = !isRecording;
  }

private:
  int getLength() {
    return getSeqLength();
  }

  void reset() {
    currentTick = 0;
    currentStep = 0;
  }

public:
  StepsPerBar stepsPerBar = 16;
  BarCount barCount = 8;

private:
  unsigned long now = 0;
  int currentTick = 0;
  byte currentStep = 0;

  byte groove = 0;

  TrackSettings* settings;
  TransportDelegate* delegate;

  unsigned long newStepTime = 0;
  bool ready = false;
  bool toBeReseted = false;
  bool isRecording = false;
  bool isPlaying = false;

  static constexpr byte stepResolution = 6;  // 24/4 -> 1 sixteenth
};
