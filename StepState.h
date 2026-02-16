
struct StepState {
  byte state = 0;  //contains the value

  static constexpr byte COUNT_BITS = 3;
  static constexpr byte POS_BITS = 2;

  static constexpr byte COUNT_SHIFT = 0;
  static constexpr byte POS_SHIFT = COUNT_SHIFT + COUNT_BITS;

  static constexpr byte COUNT_MASK = ((1 << COUNT_BITS) - 1) << COUNT_SHIFT;
  static constexpr byte POS_MASK = ((1 << POS_BITS) - 1) << POS_SHIFT;


  byte getCount() const {
    return (state & COUNT_MASK) >> COUNT_SHIFT;
  }

  void setCount(byte c) {
    state &= ~COUNT_MASK;
    state |= ((c << COUNT_SHIFT) & COUNT_MASK);
  }

  byte getPos() const {
    return (state & POS_MASK) >> POS_SHIFT;
  }

  void setPos(byte p) {
    state &= ~POS_MASK;
    state |= ((p << POS_SHIFT) & POS_MASK);
  }

  void clear() {
    state = 0;
  }
};