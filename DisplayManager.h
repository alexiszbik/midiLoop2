#pragma once

#include "Screen.h"

#define CHAR_NOTE 14
#define CHAR_PLAY 16
#define CHAR_STOP 219
#define CHAR_REC 'R'
#define CHAR_MUTE 'M'

#define BASE_FIG 48

const char numbers[100][2] PROGMEM = {
  { '0', '0' }, { '0', '1' }, { '0', '2' }, { '0', '3' }, { '0', '4' }, { '0', '5' }, { '0', '6' }, { '0', '7' }, { '0', '8' }, { '0', '9' }, { '1', '0' }, { '1', '1' }, { '1', '2' }, { '1', '3' }, { '1', '4' }, { '1', '5' }, { '1', '6' }, { '1', '7' }, { '1', '8' }, { '1', '9' }, { '2', '0' }, { '2', '1' }, { '2', '2' }, { '2', '3' }, { '2', '4' }, { '2', '5' }, { '2', '6' }, { '2', '7' }, { '2', '8' }, { '2', '9' }, { '3', '0' }, { '3', '1' }, { '3', '2' }, { '3', '3' }, { '3', '4' }, { '3', '5' }, { '3', '6' }, { '3', '7' }, { '3', '8' }, { '3', '9' }, { '4', '0' }, { '4', '1' }, { '4', '2' }, { '4', '3' }, { '4', '4' }, { '4', '5' }, { '4', '6' }, { '4', '7' }, { '4', '8' }, { '4', '9' }, { '5', '0' }, { '5', '1' }, { '5', '2' }, { '5', '3' }, { '5', '4' }, { '5', '5' }, { '5', '6' }, { '5', '7' }, { '5', '8' }, { '5', '9' }, { '6', '0' }, { '6', '1' }, { '6', '2' }, { '6', '3' }, { '6', '4' }, { '6', '5' }, { '6', '6' }, { '6', '7' }, { '6', '8' }, { '6', '9' }, { '7', '0' }, { '7', '1' }, { '7', '2' }, { '7', '3' }, { '7', '4' }, { '7', '5' }, { '7', '6' }, { '7', '7' }, { '7', '8' }, { '7', '9' }, { '8', '0' }, { '8', '1' }, { '8', '2' }, { '8', '3' }, { '8', '4' }, { '8', '5' }, { '8', '6' }, { '8', '7' }, { '8', '8' }, { '8', '9' }, { '9', '0' }, { '9', '1' }, { '9', '2' }, { '9', '3' }, { '9', '4' }, { '9', '5' }, { '9', '6' }, { '9', '7' }, { '9', '8' }, { '9', '9' }
};

class DisplayManager {
public:
  DisplayManager() {
  }

  void init(LooperEngine* engine) {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.setTextWrap(true);
    display.setTextSize(2);
    display.cp437(true);
    display.clearDisplay();
    display.drawPixel(10, 10, SSD1306_WHITE);
    this->engine = engine;
    transport = engine->getTransport();
  }

  void setInvertedColor(bool inverted) {
    if (inverted) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
  }

  void writeNbr(byte nbr) {
    char d0 = pgm_read_byte(&numbers[nbr][0]);
    char d1 = pgm_read_byte(&numbers[nbr][1]);

    if (d0 != '0') display.write(d0);
    display.write(d1);
  }

  void writeIntNbr(int nbr) {
    if (nbr >= 100) {
      byte hundreds = nbr / 100;
      display.write('0' + hundreds);
      nbr -= hundreds * 100;
    }

    char d0 = pgm_read_byte(&numbers[nbr][0]);
    char d1 = pgm_read_byte(&numbers[nbr][1]);

    display.write(d0);
    display.write(d1);
  }

  void goToLine(byte line, byte x = 0) {
    display.setCursor(x, line * SCREEN_HEIGHT / 4);
  }

  void update() {
    TrackSettings* settings = engine->getSettings();

    byte stepsPerBar = transport->stepsPerBar;

    if (line == 0 && display.count == 0) {
      display.clearDisplay();
    }
    if (line == 0) {
      byte groove = transport->getGroove();
      goToLine(0);
      setInvertedColor(true);

      writeNbr(engine->getCurrentExclusiveTrack() + 1);

      setInvertedColor(false);
      display.write(' ');
      display.write(CHAR_NOTE);

      byte barCount = transport->barCount;

      writeNbr(barCount);
      display.write(':');
      writeNbr(stepsPerBar);
      display.write(' ');
      writeNbr(groove);

    } else if (line == 16) {
      goToLine(1);
      switch (settings->trackMode) {
        case kSequence:
          display.write("SEQ");
          break;
        case kSequencePoly:
          display.write("SEQ(poly)");
          break;
        case kArp:
          display.write("ARP");
          break;
        case kHoldPoly:
          display.write("HOLD(poly)");
          break;
        default:
          break;
      }
      
    } else if (line == 24) {
      goToLine(2);
      int tempo = engine->getTempo();
      writeIntNbr(tempo);

    } else if (line == 32) {
      goToLine(3);
      display.write(transport->getIsPlaying() ? CHAR_PLAY : CHAR_STOP);
      display.write(' ');
      display.write(transport->getRecordingState() ? CHAR_REC : ' ');
      display.write(settings->isMuted ? CHAR_MUTE : ' ');

      byte step = transport->getCurrentStep();

      byte stepModPerBar = (step % stepsPerBar);
      writeNbr(1 + step / stepsPerBar);
      display.write('.');
      writeNbr(1 + stepModPerBar / 4);
      display.write('.');
      writeNbr(1 + stepModPerBar);
    }

    display.display();

    line = (line + 1) % 64;
  }

private:
  Screen display = Screen();
  bool invertedColor = false;
  LooperEngine* engine;
  Transport* transport;

  byte line = 0;
};
