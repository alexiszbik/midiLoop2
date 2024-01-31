
#include "MIDI.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

#include "LooperEngine.h"

#define MIDI_CHANNEL 12

LooperEngine looper;

/** THIS IS SUPER IMPORTANT **/

//Use this line instead for Arduino Uno or original Nano

//MIDI_CREATE_DEFAULT_INSTANCE();

/** End of the super important message **/

//counter is dirty but works fine
unsigned int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleStart(handleStart);
  MIDI.setHandleStop(handleStop);
  MIDI.setHandleClock(handleClock);

  MIDI.turnThruOn();

  MIDI.begin(MIDI_CHANNEL);

  looper.init();
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);
  if (channel == MIDI_CHANNEL && velocity > 0) {
    looper.addNote(note);
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, LOW);
}

void handleControlChange(byte channel, byte control, byte value) {
  if (channel == MIDI_CHANNEL) {
    looper.setFromCC(control, value);
  }
}

void handleStart() {
  looper.play(true);
}

void handleStop() {
  looper.play(false);
}

void handleClock() {
  looper.tick();
}

void loop() {
  MIDI.read();

  if (counter >= 300)
  {
    counter = 0;
    looper.updateFeedback(); 
  }
  counter++;
}
