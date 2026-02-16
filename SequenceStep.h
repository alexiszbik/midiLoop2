

#include "StepState.h"

class SequenceStep {
public:
  static const byte MaxPolyphony = 4;

public:
  SequenceStep() {
  }

  SequenceStep(const SequenceStep& other) {
    state.state = other.state.state;
    hold = other.hold;
    for (int i = 0; i < MaxPolyphony; i++) {
      data[i] = other.data[i];
    }
  }

public:

  void clear() {
    for (byte i = 0; i < MaxPolyphony; i++) {
      data[i] = 0;
    }
    state.clear();
    hold = false;
  }

  void set(byte note) {
    data[0] = note;
    state.setCount(1);
    state.setPos(1);
    hold = false;
  }

  void add(byte note, bool hold = false) {
    byte count = state.getCount();
    for (byte i = 0; i < count; i++) {
      if (data[i] == note) {
        return;
      }
    }
    byte pos = state.getPos();

    data[pos] = note;
    count++;
    pos++;
    count = fmin(count, MaxPolyphony);
    pos = pos % MaxPolyphony;

    state.setCount(count);
    state.setPos(pos);
    this->hold = hold;
  }

  byte getCount() {
    return state.getCount();
  }

  byte get(byte index) {
    return data[index];
  }

  bool isHold() {
    return hold;
  }

private:
  StepState state;
  bool hold = false;
  byte data[MaxPolyphony];
};
