
static constexpr byte COUNT_BITS = 3;
static constexpr byte POS_BITS = 2;

static constexpr byte COUNT_SHIFT = 0;
static constexpr byte POS_SHIFT = COUNT_SHIFT + COUNT_BITS;

static constexpr byte COUNT_MASK = ((1 << COUNT_BITS) - 1) << COUNT_SHIFT;
static constexpr byte POS_MASK = ((1 << POS_BITS) - 1) << POS_SHIFT;

class SequenceStep {
public:
  static const byte MaxPolyphony = 4;

public:
  SequenceStep() {
  }

  SequenceStep(const SequenceStep& other) {
    state = other.state;
    hold = other.hold;
    for (int i = 0; i < MaxPolyphony; i++) {
      data[i] = other.data[i];
    }
  }

public:
  byte getCountInternal() const {
    return (state & COUNT_MASK) >> COUNT_SHIFT;
  }

  void setCountInternal(byte c) {
    state &= ~COUNT_MASK;
    state |= ((c << COUNT_SHIFT) & COUNT_MASK);
  }

  byte getPosInternal() const {
    return (state & POS_MASK) >> POS_SHIFT;
  }

  void setPosInternal(byte p) {
    state &= ~POS_MASK;
    state |= ((p << POS_SHIFT) & POS_MASK);
  }
  
  void clear() {
    for (byte i = 0; i < MaxPolyphony; i++) {
      data[i] = 0;
    }
    state = 0;
    hold = false;
  }

  void set(byte note) {
    data[0] = note;
    setCountInternal(1);
    setPosInternal(1);
    hold = false;
  }

  void add(byte note, bool hold = false) {
    byte count = getCountInternal();
    for (byte i = 0; i < count; i++) {
      if (data[i] == note) {
        return;
      }
    }
    byte pos = getPosInternal();

    data[pos] = note;
    count++;
    pos++;
    count = fmin(count, MaxPolyphony);
    pos = pos % MaxPolyphony;

    setCountInternal(count);
    setPosInternal(pos);
    this->hold = hold;
  }

  byte getCount() {
    return getCountInternal();
  }

  byte get(byte index) {
    return data[index];
  }

  bool isHold() {
    return hold;
  }

private:
  byte state = 0;
  bool hold = false;
  byte data[MaxPolyphony];
};
