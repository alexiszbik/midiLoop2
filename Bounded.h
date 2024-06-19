/*
  ==============================================================================

    Bounded.h
    Created: 16 Jun 2024 9:07:04pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

template <class T> class Bounded {
public:
    Bounded(T value, T minValue, T maxValue) : value(value), minValue(minValue), maxValue(maxValue) {
        value = fmax(fmin(value, maxValue), minValue);
    }
    
    operator T() {
        return value;
    }
    
private:
    T value;
    T minValue;
    T maxValue;
};
