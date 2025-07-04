struct Range {
    int minVal;
    int maxVal;

    Range(int minV = 0, int maxV = 1023) : minVal(minV), maxVal(maxV) {}
};

struct KnobState {
    Range range;
    float prevValue = -1;
    const float threshold = 128.f;
    void (*onChange)(int);

    KnobState(Range r = Range(), void (*cb)(int) = nullptr)
        : range(r), onChange(cb) {}

    bool shouldUpdate(float currentValue) {
        if (prevValue == -1 || fabs(currentValue - prevValue) >= threshold) {
            prevValue = currentValue;
            return true;
        }
        return false;
    }
};

struct Knob {
    byte pin;
    KnobState normalState;
    KnobState shiftState;

    bool prevShiftActive = false;
    bool waitForMove = false;

    Knob(byte p,
         KnobState normal,
         KnobState shift)
        : pin(p), normalState(normal), shiftState(shift) {}

    int getValue(float rawValue, const Range& range) {
        return round((rawValue / 1023.f) * (range.maxVal - range.minVal) + range.minVal);
    }

    void check(bool shiftActive) {
        float knobValue = (float)analogRead(pin);

        if (prevShiftActive != shiftActive) {
            prevShiftActive = shiftActive;
            if (shiftActive)
                shiftState.prevValue = knobValue;
            else
                normalState.prevValue = knobValue;
            waitForMove = true;
        }

        KnobState* currentState = shiftActive ? &shiftState : &normalState;

        if (waitForMove) {
            if (currentState->shouldUpdate(knobValue)) {
                waitForMove = false;
            }
        } else {
            if (currentState->onChange) {
                currentState->onChange(getValue(knobValue, currentState->range));
            }
        }
    }
};
