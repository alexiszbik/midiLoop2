#pragma once

#include "Screen.h"

#define CHAR_NOTE 14
#define CHAR_PLAY 16
#define CHAR_STOP 219
#define CHAR_REC 'R'
#define CHAR_MUTE 'M'

#define BASE_FIG 48

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
  }

  void setInvertedColor(bool inverted) {
    if (inverted) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
  }

  void writeNbr(byte nbr) {
    byte decimal = nbr/10;
    if (decimal) {
       display.write(BASE_FIG + decimal);
    }
    display.write(BASE_FIG + (nbr % 10));
  }

  void goToLine(byte line, byte x = 0) {
    display.setCursor(x,line * SCREEN_HEIGHT/4); 
  }

  void update() {
    
    Transport* transport = engine->getTransport();
    TrackSettings* settings = engine->getSettings();

    byte stepsPerBar = settings->stepsPerBar;

    if (line == 0) {  

      display.clearDisplay();
      goToLine(0);
      setInvertedColor(true);
      
      writeNbr(/*channnel ? */ engine->getCurrentExclusiveTrack() + 1);

      setInvertedColor(false);
      display.write(' ');
      setInvertedColor(false);
      display.write(CHAR_NOTE);

      byte barCount = settings->barCount;
      
      writeNbr(barCount);
      display.write(':');
      writeNbr(stepsPerBar);
    }
    else if (line == 16) {
      goToLine(1);
      switch (settings->trackMode) {
        case kSequence :
          display.write("SEQ");
          break;
        case kSequencePoly :
          display.write("SEQ(poly)");
          break;
        case kArp :
          display.write("ARP");
          break;
        case kHoldPoly :
          display.write("HOLD(poly)");
          break;
        default :
          break;
      }
      
    }
    else if (line == 32) {
      goToLine(3);
      display.write(transport->getIsPlaying() ? CHAR_PLAY : CHAR_STOP);
      display.write(' ');
      display.write(settings->isRecording ? CHAR_REC : ' ');
      display.write(settings->isMuted ? CHAR_MUTE : ' ');

      int step = transport->getCurrentStep();

      writeNbr(1 + step/stepsPerBar);
      display.write('.');
      writeNbr(1 + (step % stepsPerBar)/4);
      display.write('.');
      writeNbr(1 + step % stepsPerBar);
    }
    
    display.display();

    line = (line + 1)%64;
  }
 
private:
   Screen display = Screen();
   bool invertedColor = false;
   LooperEngine* engine;

   byte line = 0;

};
