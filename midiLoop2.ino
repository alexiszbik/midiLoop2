
#include "MIDI.h"
#include "Mux.h"

#define KNOB_A A7
#define KNOB_B A6
#define KNOB_C A3

using namespace admux;

Mux mux(Pin(6, INPUT, PinType::Digital), Pinset(7, 8, 9, 10));

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


  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
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

int getKnobValue(byte pin, int min, int max) {
  return round(((float)analogRead(pin)/1024.f)*(max - min) + min);
}

void handleKnobValues() {
  int potA = getKnobValue(KNOB_A, 1, 8);
  int potB = getKnobValue(KNOB_B, 1, 16);
  //int potB = round((analogRead(KNOB_B) / 1023.0) * 15.0 + 1.0);

  looper.setGlobalBarCount(potA);
  looper.setGlobalStepCount(potB);
}

void loop() {
  MIDI.read();

  if (counter == 50)
  {
    int selectedChannel = -1;
    for (byte i = 0; i < 4; i++) {
      if (mux.read(i) == LOW) {
        selectedChannel = i;
      }
    }
    if (selectedChannel > -1) {
      looper.selectExclusiveTrack(selectedChannel);
    }
  }

  if (counter == 100) {
    byte currentTrack = looper.getCurrentExclusiveTrack();
    for (byte i = 0; i < 4; i++) {
      digitalWrite(i+2, currentTrack == i);
    }
  }

  if (counter >= 200)
  {
    //handleMidiThru();
    handleKnobValues();
    
    counter = 0;
    displayManager.update(); 
  }
  counter++;
}
