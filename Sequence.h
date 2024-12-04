/*
  ==============================================================================

    Sequence.h
    Created: 16 Jun 2024 8:24:20pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "TrackSettings.h"
#include "SequenceStep.h"

class Sequence {

public:
    Sequence() {
        clearAll();
    }
    
public:
    void clearAll() {
        for (int i = 0; i < Size; i++) {
            data[i].clear();
        }
    }

    void fill(int stepCount) {
        for (int i = stepCount; i < Size; i++) {
            data[i] = data[i % stepCount];
        }
    }
public:
    SequenceStep operator [] (int i) const {return data[i];}
    SequenceStep& operator [] (int i) {return data[i];}
private:
    static const int Size = StepsPerBar::Max * BarCount::Max;
    SequenceStep data[Size];
};
