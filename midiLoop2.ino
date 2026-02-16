#include "MIDI.h"
#include "Mux.h"

#include "Switch.h"
#include "Knob.h"
#include "MIDITable.h"

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

Knob knobA(
  KNOB_A,
  KnobState(Range(), [](byte v) {  }),
  KnobState(Range(1, 8), [](byte v) { looper.setGlobalBarCount(v); })
);

Knob knobB(
  KNOB_B,
  KnobState(Range(), [](byte v) {  }),
  KnobState(Range(), [](byte v) { looper.setGlobalStepCount(v); })
);

Knob knobC(
  KNOB_C,
  KnobState(Range(), [](byte v) {  }),
  KnobState(Range(0, 50), [](byte v) { looper.setGroove(v); })
);

enum UpdateStep : byte {
  kRecSwitch = 0,
  kOtherSwitches,
  kRecLed,
  kSelectedChannelSwitches,
  kSelectedChannelLeds,
  kKnobs,

  kUpdateStepCount
};

byte currentUpdateStep = 0;
byte counter = 0;
bool shiftState = false;

void setup() {
  delay(200);
  Serial.begin(9600);
  Serial1.begin(31250); //MIDI baud rate

  for (byte i = 0; i < TRACK_COUNT; i++) {
    pinMode(LED_TRACK_SELECT+i, OUTPUT); 
    pinMode(LED_TRACK_PLAY+i, OUTPUT); 
  }

  pinMode(LED_REC, OUTPUT);
  pinMode(LED_PLAY, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.setHandleStart(handleStart);
  MIDI.setHandleStop(handleStop);
  MIDI.setHandleClock(handleClock);

  MIDI.turnThruOn();

  MIDI.begin(MIDI_CHANNEL_OMNI); //check if there's no performance issues

  displayManager.init(&looper);
}

void handleNoteOn(byte channel, byte note, byte velocity) {
    if (channel == MIDI_CHANNEL) {
        looper.playNoteOn(note, velocity);
    }
    if (channel == LOOPER_CHANNEL) {
        
        if (note >= SELECT_CHANNEL_NOTE && note < (SELECT_CHANNEL_NOTE + 4)) {
            byte channel = note - SELECT_CHANNEL_NOTE;
            looper.selectExclusiveTrack(channel);
        }
        else if (note >= MUTE_CHANNEL_NOTE && note < (MUTE_CHANNEL_NOTE + 4)) {
            byte channel = note - MUTE_CHANNEL_NOTE;
            looper.getTrackSettings(channel)->isMuted = (velocity > 0);
        
        }  else if (note == ERASE_ALL_NOTE) {
            if (velocity > 0) {
                looper.clearAllTracks();
            }
        }
    }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    if (channel == MIDI_CHANNEL) {
        looper.playNoteOn(note, 0);
    }
    if (channel == LOOPER_CHANNEL) {
        if (note >= MUTE_CHANNEL_NOTE && note < (MUTE_CHANNEL_NOTE + 4)) {
            byte channel = note - MUTE_CHANNEL_NOTE;
            looper.getTrackSettings(channel)->isMuted = false;
        } 
    }
}

void handleControlChange(byte channel, byte control, byte value) {
    if (channel == MIDI_CHANNEL) {
        if (control == 1) { // modulation wheel ?
            //MIDI.sendControlChange(control, value, 5);
            looper.setModulationWheel(value);
        }
        //looper.controlChange(control, value);
    }
    if (channel == LOOPER_CHANNEL) {
        if (control == BAR_COUNT_CC) {
            looper.setGlobalBarCount(value);
        } else if (control == ARP_ONOFF_CC) {
            bool useArp = (value >= 64);
            looper.setTrackMode(useArp ? kArp : kSequence);
            //arpState.panic(); //Maybe it is safer
        } else if (control == SEQ_FILL_CC) {
            looper.fill();
        } else if (control == REC_ONOFF_CC) {
            bool rec = (value >= 64);
            looper.setIsRecording(rec);
        } else if (control >= MUTE_CHANNEL_CC && control < (MUTE_CHANNEL_CC + 4)) {
            byte channel = control - MUTE_CHANNEL_CC;
            looper.getTrackSettings(channel)->isMuted = (value > 64);
        } else if (control == COPY_CC) {
            looper.copy(value);
        } else if (control == PASTE_CC) {
            looper.paste(value);
        }
    }
}

void handlePitchBend(byte channel, int bend) {
  if (channel == MIDI_CHANNEL) {
    //TODO : handle pitch bend
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

void handleKnobValues() {
  knobA.check(shiftState);
  knobB.check(shiftState);
  knobC.check(shiftState);
}

void updatePlayLed() {
  Transport* tranport = looper.getTransport();
  bool isQuarter = (tranport->getCurrentStep() % 4) == 0;
  isQuarter = isQuarter && tranport->getIsPlaying();
  digitalWrite(LED_PLAY, isQuarter ? HIGH : LOW);
}

void updateRecLed() {
  Transport* transport = looper.getTransport();
  digitalWrite(LED_REC, transport->getRecordingState() ? HIGH : LOW);
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
            case 1 :
              looper.toggleMute();
              break;
            case 2 :
              looper.copyPaste();
              break;
            case 3 :
              looper.fill();
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
  looper.loop();
  
  if (counter > 60) {
    switch(currentUpdateStep) {

      case kRecSwitch : updateRecSwitch(); break;

      case kOtherSwitches : updateOtherSwitches(); displayManager.update(); break;

      case kRecLed : updateRecLed(); break;

      case kSelectedChannelSwitches : updateSelectedChannelSwitches(); displayManager.update(); break;

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
