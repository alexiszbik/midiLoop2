
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

  virtual void load(byte pitch) {
    if (!contains(pitch)) {
      if (size < MAX_SIZE) {
        data[size] = pitch;
        //Serial.println(pitch);

        //midiOut->sendNote(trackIndex, settings->channelOut, pitch, MAX_VELOCITY);

        size++;
      }
    }
  }

  void processNotesOn() {
    if (size == 0) return;
    for (byte i = 0; i < size; i++) {
      if (data[i] & 0b10000000) continue;  //is sent ? if the first note has been sent, we assume that every notes has been sent !

      byte note = data[i] & 0b01111111;  //binary mask

      midiOut->sendNote(
        trackIndex,
        settings->channelOut,
        note,
        MAX_VELOCITY);

      data[i] |= 0b10000000;  // mark has sent
    }
  }

  void sendNotesOff() {
    for (byte i = 0; i < size; i++) {
      byte note = data[i] & 0b01111111;  //binary mask
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