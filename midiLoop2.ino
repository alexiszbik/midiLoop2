
#include "MIDI.h"

//for nano every
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

#define MIDI_CHANNEL 1

/** THIS IS SUPER IMPORTANT **/

//Use this line instead for Arduino Uno or original Nano

//MIDI_CREATE_DEFAULT_INSTANCE();

/** End of the super important message **/


#include "MidiOut.h"
#include "LooperEngine.h"
#include "DisplayManager.h"

MidiOut midiOut;
LooperEngine looper = LooperEngine(&midiOut);
DisplayManager displayManager;

//counter is dirty but works fine
unsigned int counter = 0;

#define REC_SWITCH_TEST 3
//#define ARP_SWITCH_TEST 4

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  //Serial1.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleStart(handleStart);
  MIDI.setHandleStop(handleStop);
  MIDI.setHandleClock(handleClock);

  MIDI.turnThruOn();

  MIDI.begin(MIDI_CHANNEL);

  displayManager.init(&looper);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, HIGH);
  if (channel == MIDI_CHANNEL) {
    looper.playNoteOn(note, velocity);
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_BUILTIN, LOW);
  if (channel == MIDI_CHANNEL) {
    looper.playNoteOn(note, 0);
  }
}

void handleControlChange(byte channel, byte control, byte value) {
  if (channel == MIDI_CHANNEL) {
    //looper.controlChange(control, value);
  }
}

void handleStart() {
  looper.setIsPlaying(true);
}

void handleStop() {
  looper.setIsPlaying(false);
}

void handleClock() {
  looper.tick();
}

void loop() {
  MIDI.read();

  if (counter >= 200)
  {
    //handleMidiThru();
    counter = 0;
    displayManager.update(); 
  }
  counter++;
}
