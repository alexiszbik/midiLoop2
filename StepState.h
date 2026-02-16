
struct StepState {
  byte state = 0;  //contains the value

  static constexpr byte COUNT_BITS = 3;
  static constexpr byte POS_BITS = 2;
  static constexpr byte HOLD_BITS = 1;

  static constexpr byte COUNT_SHIFT = 0;
  static constexpr byte POS_SHIFT = COUNT_SHIFT + COUNT_BITS;
  static constexpr byte HOLD_SHIFT = POS_SHIFT + POS_BITS;

  static constexpr byte COUNT_MASK = ((1 << COUNT_BITS) - 1) << COUNT_SHIFT;
  static constexpr byte POS_MASK = ((1 << POS_BITS) - 1) << POS_SHIFT;
  static constexpr byte HOLD_MASK = ((1 << HOLD_BITS) - 1) << HOLD_SHIFT;


  byte getCount() const {
    return (state & COUNT_MASK) >> COUNT_SHIFT;
  }

  void setCount(byte c) {
    state &= ~COUNT_MASK;
    state |= ((c << COUNT_SHIFT) & COUNT_MASK);
  }

  byte getPos() const {
    return ((state & POS_MASK) >> POS_SHIFT);
  }

  void setPos(byte p) {
    state &= ~POS_MASK;
    state |= ((p << POS_SHIFT) & POS_MASK);
  }

  byte getHold() const {
    return ((state & HOLD_MASK) >> HOLD_SHIFT);
  }

  void setHold(byte p) {
    state &= ~HOLD_MASK;
    state |= ((p << HOLD_SHIFT) & HOLD_MASK);
  }

  void clear() {
    state = 0;
  }
};