#ifndef SWITCH_H
#define SWITCH_H

using namespace admux;

class Switch {
public:
  Switch(byte pin, bool inverted = false) : pin(pin), inverted(inverted) {
  }

  bool debounce() {
    byte readValue = mux.read(pin);
    bool currentState = inverted ? (readValue == HIGH) : (readValue == LOW);
    if (currentState != state) {
      state = currentState;
      stateChanged = true;
    }
    
    return stateChanged;
  }

  bool getState() {
    stateChanged = false;
    return state;
  }

private:
  byte pin;
  bool analogPin;
  bool inverted;

  bool state = false;
  bool stateChanged = false;
};


#endif //SWITCH_H