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

  void tick(int tempo) {

    if (!isPlaying) return;

    if (currentTick % stepResolution == 0) {

      if (toBeReseted) {
        reset();
        toBeReseted = false;
      } else {
        currentStep = currentTick / stepResolution;
      }

      bool isOddStep = currentStep & 1;

      unsigned long stepMs = 60000UL / tempo / 4;  // 1/16
      unsigned long offset = isOddStep ? (stepMs * groove) / 100 : 0;

      newStepTime = now + offset;
      ready = true;
    }

    currentTick++;
    currentTick = currentTick % getSeqLength();
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

    int baseStep = currentTick / stepResolution;
    int tickOffset = currentTick % stepResolution;

    bool quantizeDown = tickOffset < (stepResolution / 2);
    *playNote = quantizeDown && tickOffset != 0;

    int step = quantizeDown ? baseStep : baseStep + 1;

    if (step >= stepCount) step -= stepCount;
    return step;
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

  void setRecordingState(bool r) {
    isRecording = r;
  }

  void toggleRecording() {
    isRecording = !isRecording;
  }

private:
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
