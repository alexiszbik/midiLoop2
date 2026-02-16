

#include "StepState.h"

class SequenceStep {
public:
  static const byte MaxPolyphony = 4;

public:
  SequenceStep() {
    clear();
  }

  SequenceStep(const SequenceStep& other) {
    state.state = other.state.state;
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
  }

  void set(byte note) {
    data[0] = note;
    state.setCount(1);
    state.setPos(1);
    state.setHold(false);
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
    state.setHold(hold);
  }

  byte getCount() {
    return state.getCount();
  }

  byte get(byte index) {
    return data[index];
  }

  bool isHold() {
    return state.getHold();
  }

private:
  StepState state;
  byte data[MaxPolyphony];
};
