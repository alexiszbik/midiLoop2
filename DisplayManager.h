#pragma once

#include "Screen.h"

#include "LooperData.h"

#define CHAR_NOTE 14
#define CHAR_PLAY 16
#define CHAR_STOP 219
#define CHAR_REC 'R'

#define BASE_FIG 48

class DisplayManager {
public:
  DisplayManager() {
  }

  void init(LooperData* data) {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.setTextWrap(true);
    display.setTextSize(2); 
    display.cp437(true); 
    display.clearDisplay();
    this->data = data;
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

    byte chan = data->seq.selectedChannel;
    Track* t = &data->seq.tracks[chan - 1];
    
    if (line == 0) {  

      display.clearDisplay();
      goToLine(0);
      setInvertedColor(true);
      
      writeNbr(chan);

      setInvertedColor(false);
      display.write(' ');
      setInvertedColor(false);
      display.write(CHAR_NOTE);
      
      writeNbr(t->barCount);
      display.write(':');
      writeNbr(t->stepsPerBar);
    }
    else if (line == 32) {
      goToLine(3);
      display.write(data->isPlaying ? CHAR_PLAY : CHAR_STOP);
      display.write(' ');
      display.write(data->isRecording ? CHAR_REC : ' ');
      display.write(' ');

      writeNbr(t->currentBar);
      display.write('.');
      writeNbr(t->currentStep/4);
      display.write('.');
      writeNbr(t->currentStep);
    }
    
    display.display();

    line = (line + 1)%64;
  }
 
private:
   Screen display = Screen();
   bool invertedColor = false;
   LooperData* data;

   byte line = 0;

};
