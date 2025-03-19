MIDI LOOPER v2

Warning : Library depedencies
You will need those libraries to build the looper.

### arduino-ad-mux-lib v1.1.14
https://github.com/stechio/arduino-ad-mux-lib

### Adafruit_SSD1306 v2.5.9 
https://github.com/adafruit/Adafruit_SSD1306
I rewrote the Screen.h for this library in order to have less latency with MIDI. I didn't test with newer version.

### arduino_midi_library
https://github.com/FortySevenEffects/arduino_midi_library

more informations soon ...
For MIDI messages, check MidiTable.h

/!\ This project doesn't send any MIDI clock by itself, you will need a sync clock provider plugged on MIDI IN

Schematic now available :

![alt text](https://github.com/alexiszbik/midiLoop2/blob/main/midiloop2%20_schematic.png?raw=true)

1x Arduino Nano Every  
1x OLED Screen SSD1306 (128*64)  
1x 74HC4067 16-chan analog multiplexer  
4x arcade buttons  
6x LEDs  
4x buttons  
1x 1n4148   
1x 6n138 opto optocoupler  
2x MIDI female sockets  
3x potentiometers (10k is fine)  
1x 470R  
1x 10k  
13x 220R  
1x 1k  


