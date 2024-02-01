
#include "Const.h"
#include "serialMIDI.h"
#include "Arpeggiator.h"

#define STEP_LENGTH 6
#define SEQUENCE_LENGTH_MAX MAX_BARS*MAX_STEPS_PER_BAR

#define ON 127
#define OFF 0

class Track {
public:
  Track() {
    clearAll();
  }

public:
  void tick() {
    if (currentTick % STEP_LENGTH == 0) {
      triggerNoteOff();
      triggerNoteOn();
    }
    incrementTick(); 
  }

  void resetTicks() {
    currentTick = 0;
    currentStep = 0;
    currentBar = 0;
  }

  void noteOn(byte note) {
    if (arpState) {
      arp.addNote(note);
    } else {
      sequence[stepNext()] = note;
    }
  }

  void noteOff(byte note) {
    if (arpState) {
      arp.removeNote(note);
    } else {

    }
  }

  void clear() {
    sequence[stepNext()] = 0;
  }

  void clearAll() {
    for (int i = 0; i < SEQUENCE_LENGTH_MAX; i++) {
      sequence[i] = 0;
    }
  }

  void triggerNoteOff() {
    if (previousNote > 0) {
      MIDI.sendNoteOn(previousNote, OFF, midiChannel);
      previousNote = 0;
    }
  }

private:
  int stepNow() {
    return currentBar * stepsPerBar + currentStep;
  }

  int stepNext() {
    return (stepNow() + 1) % (barCount * stepsPerBar);
  }

  void incrementTick() {
    currentTick++;
    if (currentTick % STEP_LENGTH == 0) {
      currentStep++;
    }
    if (currentStep >= stepsPerBar) {
      currentStep = 0;
      currentBar++;
    }
    if (currentBar >= barCount) {
      currentBar = 0;
    }

    currentTick = currentTick % 96;
  }

  void triggerNoteOn() {
    byte note = sequence[stepNow()];

    if (note > 0 && !isMuted) {
      MIDI.sendNoteOn(note, ON, midiChannel);
      previousNote = note;
    }
  }

public:
  byte midiChannel = 1;
  bool isMuted = false;

  bool arpState = false;

  byte barCount = 4;
  byte stepsPerBar = MAX_STEPS_PER_BAR;

  byte currentStep = 0;
  byte currentBar = 0;

private:
  byte currentTick = 0;

  byte sequence[SEQUENCE_LENGTH_MAX];
  byte previousNote = 0;

  Arpeggiator arp;
  
};