#pragma once

#define DEFAULT_TEMPO 120

class Tempo {

public:
  Tempo() {
    for (byte i = 0; i < nbBpm; i++) {
      bpmList[i] = DEFAULT_TEMPO;
    }
    sum = nbBpm * DEFAULT_TEMPO;
  }

  void tick() {
    unsigned long newTime = millis();
    unsigned long delta = newTime - lifeTime;
    lifeTime = newTime;

    if (delta > 0) {
      int bpmNow = 60000 / (24 * delta);

      sum -= bpmList[bpmListIndex];  // enlève ancienne valeur
      sum += bpmNow;                 // ajoute nouvelle

      bpmList[bpmListIndex] = bpmNow;

      bpmListIndex++;
      if (bpmListIndex >= nbBpm) bpmListIndex = 0;
      
      int bpm = sum / nbBpm;
      tempo = (int)bpm;
    }
  }

  int getTempo() {
    return tempo;
  }

private:
  unsigned long lifeTime = 0;

  static constexpr int nbBpm = 24;
  int bpmList[nbBpm];
  byte bpmListIndex = 0;
  int tempo = DEFAULT_TEMPO;
  long sum;
};
