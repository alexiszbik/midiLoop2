
#include "MIDI.h"

#include "LooperEngine.h"

#define MIDI_CHANNEL 12

LooperEngine looper;

/** THIS IS SUPER IMPORTANT **/

//Use this line instead for Arduino Uno or original Nano

//MIDI_CREATE_DEFAULT_INSTANCE();

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

/** End of the super important message **/

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  looper.init();
  
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  //displayManager.print("OFF");
  digitalWrite(LED_BUILTIN, LOW);
}


void handleControlChange(byte channel, byte control, byte value) {
  if (channel == MIDI_CHANNEL) {
    looper.setFromCC(control, value);
  } else {
    MIDI.sendControlChange(control, value, channel);
  }
}

void loop() {
  MIDI.read();
}
