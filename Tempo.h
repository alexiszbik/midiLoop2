#pragma once

#define DEFAULT_TEMPO 120

class Tempo {

public:
  Tempo() {
    for (byte i = 0; i < nbBpm; i++) {
      bpmList[i] = DEFAULT_TEMPO;
    }
  }

  void tick() {
    unsigned long newTime = millis();
    unsigned long delta = newTime - lifeTime;
    lifeTime = newTime;

    if (delta > 0) {
      float bpmNow = 60000.0 / (24.0 * delta);
      bpmList[bpmListIndex] = bpmNow;
      bpmListIndex = (bpmListIndex + 1) % nbBpm;

      float sum = 0;
      for (byte i = 0; i < nbBpm; i++) {
        sum += bpmList[i];
      }
      float bpm = sum / nbBpm;
      tempo = (int)bpm;
    }
  }

  int getTempo() {
    return tempo;
  }

private:
  unsigned long lifeTime = 0;

  static constexpr int nbBpm = 24;
  float bpmList[nbBpm];
  byte bpmListIndex = 0;
  int tempo = DEFAULT_TEMPO;
};
