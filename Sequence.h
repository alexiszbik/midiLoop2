/*
  ==============================================================================

    Sequence.h
    Created: 16 Jun 2024 8:24:20pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include "TrackSettings.h"

class Sequence {
public:
    Sequence() {
        clearAll();
    }
    
public:
    void clearAll() {
        for (int i = 0; i < Sequence::Size; i++) {
            data[i] = 0;
        }
    }
public:
    byte operator [] (int i) const {return data[i];}
    byte& operator [] (int i) {return data[i];}
private:
    static const int Size = StepsPerBar::Max * BarCount::Max;
    byte data[Sequence::Size];
};
