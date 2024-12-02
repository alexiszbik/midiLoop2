
#pragma once

class NotePool {

public:
  NotePool(byte maxSize = 10) : maxSize(maxSize) {
    data = (byte*)malloc(sizeof(byte) * maxSize);
  }

protected: 
    bool contains(byte note) {
      for (byte i = 0; i < size; i++) {
        if (data[i] == note) {
          return true;
        }
      }
      return false;
    }

public:
  virtual void clear() {
    for (byte i = 0; i < size; i++) {
      data[i] = 0;
    }
    size = 0;
  }

  virtual void add(byte pitch) {
    if (!contains(pitch)) {
      if (size < maxSize) {
        data[size] = pitch;
        size++;
      }
    }
  }

  byte getSize() {
    return size;
  }

  byte get(byte pos) {
    return data[pos];
  }

protected:
  byte* data = nullptr;
  byte maxSize;
  byte size = 0;

};