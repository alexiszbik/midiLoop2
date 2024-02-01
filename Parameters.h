
#include <EEPROM.h>
#include "Const.h"
#include "Macros.h"

struct Bounds {
  byte min;
  byte max;
};

class ParametersDelegate {
public:
  virtual ~ParametersDelegate() {};
  virtual void parameterChanged(Param p, byte value) = 0;
};

class Parameters {

public:
  Parameters(ParametersDelegate* delegate) : delegate(delegate) {
  }

  void readAll() {
    for (int addr = 0; addr < Param::count; addr++) {
      vars[addr] = EEPROM.read(addr);
      vars[addr] = mapValue(addr, vars[addr]);
      delegate->parameterChanged((Param)addr, vars[addr]);
    }
  }

  byte get(Param p) {
    return vars[p];
  }

  void set(Param p, byte value) {
    if (value != get(p)) {
      vars[p] = value;
      EEPROM.write(p, value);
      delegate->parameterChanged(p, value);
    }
  }

  byte mapValue(Param p, byte value) {
    Bounds bounds = getBounds(p);
    return constrain(value, bounds.min, bounds.max);
  }

  Bounds getBounds(Param p) {
     switch (p) {
      case channelSelect :
        return {1,TRACK_COUNT};

      _allTrackCase(barCount) : 
        return {1,MAX_BARS};

      _allTrackCase(stepCount) : 
        return {1,MAX_STEPS_PER_BAR};

      _allTrackCase(arpState) : 
        return {0,1};
    }
  }

  void setFromCC(byte control, byte value) {
    int addr = (int)control - CC_START;

    if (addr < 0 || addr >= Param::count) {
      return;
    }

    switch (addr) {
      _allTrackCase(arpState) : 
        value = midiValueToBool(value);
        break;

    default:
      break;
    }

    set(addr, mapValue(addr, value));
  }

private:
  byte vars[Param::count];

  ParametersDelegate* delegate = nullptr;
};




