
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

public:
  byte selectedChannel = 1;
  Track tracks[TRACK_COUNT];

};