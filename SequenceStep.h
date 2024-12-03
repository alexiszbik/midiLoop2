
class SequenceStep {
public:
  static const byte MaxPolyphony = 4;

public:
  void clear() {
    for (byte i = 0; i < MaxPolyphony; i++) {
      data[i] = 0;
    }
    count = 0;
    pos = 0;
    hold = false;
  }

  void set(byte note) {
    data[0] = note;
    count = 1;
    pos = 1;
    hold = false;
  }

  void add(byte note, bool hold = false) {
    for (byte i = 0; i < count; i++) {
      if (data[i] == note) {
        return;
      }
    }
    data[pos] = note;
    count++;
    pos++;
    count = fmin(count, MaxPolyphony);
    pos = pos % MaxPolyphony;
    this->hold = hold;
  }

  byte getCount() {
    return count;
  }

  byte get(byte index) {
    return data[index];
  }

  bool isHold() {
    return hold;
  }

private:
  byte count = 0;
  byte pos = 0;
  bool hold = false;
  byte data[MaxPolyphony];

};