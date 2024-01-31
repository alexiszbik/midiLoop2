
#include "Const.h"
#include "Track.h"

class Sequencer {
public:
  Sequencer() {
    for (byte i = 0; i < TRACK_COUNT; i++) {
      tracks[i].midiChannel = i + 1;
    }
  }

public:
  void tick() {
    for (byte i = 0; i < TRACK_COUNT; i++) {
      tracks[i].tick();
    }  
  }

  void resetTicks() {
    for (byte i = 0; i < TRACK_COUNT; i++) {
      tracks[i].resetTicks();
    }  
  }

  void addNote(byte note) {
    tracks[selectedChannel - 1].addNote(note);
  }

  void triggerNoteOff() {
    for (byte i = 0; i < TRACK_COUNT; i++) {
      tracks[i].triggerNoteOff();
    }  
  }

  void noteThru(byte note, byte velocity) {
    byte channel = tracks[selectedChannel - 1].midiChannel;
    if (velocity > 0) {
      MIDI.sendNoteOn(note, velocity, channel);
    } else {
      MIDI.sendNoteOff(note, 0, channel);
    }
  }

  void clearAll() {
    for (byte i = 0; i < TRACK_COUNT; i++) {
      tracks[i].clearAll();
    }
  }

public:
  byte selectedChannel = 1;
  Track tracks[TRACK_COUNT];

};