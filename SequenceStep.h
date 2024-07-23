
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
  }

  void set(byte note) {
    data[0] = note;
    count = 1;
    pos = 1;
  }

  void add(byte note) {
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
  }

  byte getCount() {
    return count;
  }

  byte get(byte index) {
    return data[index];
  }

private:
  byte count = 0;
  byte pos = 0;
  byte data[MaxPolyphony];
};