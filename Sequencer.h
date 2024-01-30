
#include "Track.h"

#define TRACK_COUNT 4

class Sequencer {

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

public:
  Track tracks[TRACK_COUNT];

};