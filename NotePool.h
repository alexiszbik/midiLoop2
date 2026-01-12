
#pragma once

#define MAX_SIZE 4

class NotePool {

public:
  NotePool() {
  }

  void initialize(MidiOut* midiOut, byte trackIndex, TrackSettings* settings) {
    this->trackIndex = trackIndex;
    this->midiOut = midiOut,
    this->settings = settings;
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
      if (size < MAX_SIZE) {
        data[size] = pitch;
        size++;
      }
    }
    midiOut->sendNote(trackIndex, settings->channelOut, pitch, MAX_VELOCITY);
  }

  byte getSize() {
    return size;
  }

  byte get(byte pos) {
    return data[pos];
  }

  void sendNotesOff() {
    for (byte i = 0; i < getSize(); i++) {
      byte note = get(i);
      midiOut->sendNote(trackIndex, settings->channelOut, note, 0);
    }
    clear();
  }

protected:
  byte trackIndex = 0;
  byte data[MAX_SIZE];
  byte size = 0;
  MidiOut* midiOut = nullptr;
  TrackSettings* settings = nullptr;

};