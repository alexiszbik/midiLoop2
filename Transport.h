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
    virtual ~TransportDelegate() {};
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
                  currentStep = currentTick/settings->getStepResolution();
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
        float stepRatio = (float)currentTick/(float)settings->getStepResolution();
        float ratioFrac = stepRatio - floor(stepRatio);
        bool quantizeDown = ratioFrac < 0.5;
        *playNote = quantizeDown;
        int step = quantizeDown ? floor(stepRatio) : ceil(stepRatio);
        return (step + stepCount) % stepCount;
    }

    int getCurrentStep() {
        return currentStep;
    }

    void willReset() {
      toBeReseted = true;
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
    int currentStep = 0;
    bool isPlaying = false;
    bool toBeReseted = false;
    
    TrackSettings* settings;
    TransportDelegate* delegate;
};


