

class NotePool {

public:
  NotePool(byte maxSize) : maxSize(maxSize) {
    data = (byte*)malloc(sizeof(byte) * maxSize);
  }
public:
  void clear() {
    for (byte i = 0; i < size; i++) {
      data[i] = 0;
    }
    size = 0;
  }

  void add(byte pitch) {
    if (size < maxSize) {
      data[size] = pitch;
      size++;
    }
  }

  byte getSize() {
    return size;
  }

  byte get(byte pos) {
    return data[pos];
  }

private:
  byte* data;
  byte maxSize;
  byte size = 0;

};