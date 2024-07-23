#include "MIDI.h"
#include "Mux.h"

using namespace admux;
Mux mux(Pin(6, INPUT, PinType::Digital), Pinset(7, 8, 9, 10));

#include "Switch.h"

#define KNOB_A A7
#define KNOB_B A6
#define KNOB_C A3

#define LED_PLAY 11
#define LED_REC 12

#define LED_TRACK_SELECT 2

#define LED_TRACK_PLAY 13

#define MUX_BUTTON_PLAY 4
#define MUX_BUTTON_CLEAR 5
#define MUX_BUTTON_SHIFT 6
#define MUX_BUTTON_REC 7

//We use Serial1 for Nano Every
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

#define MIDI_CHANNEL 1

#include "MidiOut.h"
#include "LooperEngine.h"
#include "DisplayManager.h"

MidiOut midiOut;
LooperEngine looper = LooperEngine(&midiOut);
DisplayManager displayManager;

Switch recSwitch = Switch(MUX_BUTTON_REC);
Switch shiftSwitch = Switch(MUX_BUTTON_SHIFT, true);
Switch clearSwitch = Switch(MUX_BUTTON_CLEAR, true);
Switch playSwitch = Switch(MUX_BUTTON_PLAY, true);

Switch channelSwitches[TRACK_COUNT] = {Switch(0), Switch(1), Switch(2), Switch(3)};

enum UpdateStep {
  kRecSwitch = 0,
  kOtherSwitches,
  kRecLed,
  kSelectedChannelSwitches,
  kSelectedChannelLeds,
  kKnobs,

  kUpdateStepCount
};

byte currentUpdateStep = 0;
unsigned int counter = 0;
bool shiftState = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  for (byte i = 0; i < TRACK_COUNT; i++) {
    pinMode(LED_TRACK_SELECT+i, OUTPUT); 
    pinMode(LED_TRACK_PLAY+i, OUTPUT); 
  }

  pinMode(LED_REC, OUTPUT);
  pinMode(LED_PLAY, OUTPUT);

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
  if (channel == MIDI_CHANNEL) {
    looper.playNoteOn(note, velocity);
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
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
  updatePlayLed();
  for (byte i = 0; i < 4; i++) {
    digitalWrite(LED_TRACK_PLAY+i, LOW);
  }
}

void handleClock() {
  looper.tick();
  updatePlayLed();
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

void updatePlayLed() {
  Transport* tranport = looper.getTransport();
  bool isQuarter = (tranport->getCurrentStep() % 4) == 0;
  isQuarter = isQuarter && tranport->getIsPlaying();
  digitalWrite(LED_PLAY, isQuarter ? HIGH : LOW);
}

void updateRecLed() {
  TrackSettings* settings = looper.getSettings();
  digitalWrite(LED_REC, settings->isRecording ? HIGH : LOW);
}

void updateRecSwitch() {
  if (recSwitch.debounce()) {
    if (recSwitch.getState()) {
      looper.toggleIsRecording();
    }
  }
}

void updateSelectedChannelSwitches() {

  int selectedChannel = -1;
  for (byte i = 0; i < TRACK_COUNT; i++) {
    if (channelSwitches[i].debounce()) {
      if (channelSwitches[i].getState() == true) {
        if (shiftState) {
          switch(i) {
            case 0 :
              looper.toggleTrackMode();
              break;
            default :
              break;
          }
        } else {
          selectedChannel = i;
        }
        
      }
    }
  }
  if (selectedChannel > -1) {
    looper.selectExclusiveTrack(selectedChannel);
  }
}


void updateOtherSwitches() {
  if (shiftSwitch.debounce()) {
    shiftState = shiftSwitch.getState();
  }
  if (clearSwitch.debounce()) {
    bool clearState = clearSwitch.getState();
    if (shiftState) {
      if (clearState) {
        looper.clearAll();
      }
    } else {
      looper.setEraserState(clearState);
    } 
  }
  if (playSwitch.debounce()) {
    bool playState = playSwitch.getState();
    if (shiftState) {
      if (playState) {
        looper.resetTransport();
      }
    }
  }
}

void updateSelectedChannelLeds() {
  byte currentTrack = looper.getCurrentExclusiveTrack();
  for (byte i = 0; i < TRACK_COUNT; i++) {
    digitalWrite(LED_TRACK_SELECT+i, currentTrack == i);
  }
}

void loop() {
  MIDI.read();
  
  if (counter > 20) {
    switch(currentUpdateStep) {

    case kRecSwitch : updateRecSwitch(); break;

    case kOtherSwitches : updateOtherSwitches(); break;

    case kRecLed : updateRecLed(); break;

    case kSelectedChannelSwitches : updateSelectedChannelSwitches(); break;

    case kSelectedChannelLeds : updateSelectedChannelLeds(); break;

    case kKnobs : handleKnobValues(); break;

    default : break;
    }

    if (currentUpdateStep == kUpdateStepCount) {
      displayManager.update();
      currentUpdateStep = 0;
    } else {
      currentUpdateStep++;
    }
    counter = 0;
  } else {
    counter++;
  }
}
