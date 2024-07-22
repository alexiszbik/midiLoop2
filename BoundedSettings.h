#include "Bounded.h"

class StepsPerBar : public Bounded<byte> {
public:
    static const byte Min = 1;
    static const byte Max = 16;
public:
    StepsPerBar(byte value) : Bounded(value, Min, Max) {}
};

class BarCount : public Bounded<byte> {
public:
    static const byte Min = 1;
    static const byte Max = 8;
public:
    BarCount(byte value) : Bounded(value, Min, Max) {}
};