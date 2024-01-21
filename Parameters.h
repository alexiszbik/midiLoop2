
#include <EEPROM.h>

#define CC_START 10

#define _allTrackEnum(ParamName) \
ParamName##1, \
ParamName##2, \
ParamName##3, \
ParamName##4

#define _allTrackCase(ParamName) \
case ParamName##1 : \
case ParamName##2 : \
case ParamName##3 : \
case ParamName##4 

enum Param {
  channelSelect,
  _allTrackEnum(barCount),
  _allTrackEnum(stepCount),

  count
};

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
    for (int addr = 0; addr << Param::count; addr++) {
      vars[addr] = EEPROM.read(addr);
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

  void setClamped(Param p, byte value, Bounds bounds) {
    set(p, constrain(value, bounds.min, bounds.max));
  }

  Bounds getBounds(Param p) {
     switch (p) {
      case channelSelect :
        return {1,4};

      _allTrackCase(barCount) : 
        return {1,16};
    }
  }

  void setFromCC(byte addrFromCC, byte ccValue) {
    int addr = (int)addrFromCC - CC_START;

    if (addr < 0 || addr >= Param::count) {
      return;
    }

    setClamped(addr, ccValue, getBounds(addr));
  }

private:
  byte vars[Param::count];

  ParametersDelegate* delegate = nullptr;
};




