#include "Parameters.h"
#include "DisplayManager.h"
#include "Sequencer.h"

class LooperEngine : public ParametersDelegate {
public:
  LooperEngine() {
  }

  void init() {
    displayManager.init();
    parameters.readAll();
  }

  void updateDisplay() {
    String str = "chan:" + String(selectedChannel);
    str += "\nb:" + String(seq.tracks[selectedChannel - 1].barCount);
    displayManager.print(str.c_str());
  }

  byte getTrackId(Param p, Param firstTrackP) {
    return p - firstTrackP;
  }

  virtual void parameterChanged(Param p, byte value) override {
    switch(p) {
      case channelSelect: {
        selectedChannel = value;
      }
      break;

      _allTrackCase(barCount) :  {
        byte trackId = getTrackId(p, barCount1);
        seq.tracks[trackId].barCount = value;
      }
      break;
    }

    updateDisplay();
  }

  void setFromCC(byte addrFromCC, byte ccValue) {
    parameters.setFromCC(addrFromCC, ccValue);
  }

private:
  Parameters parameters = Parameters(this);  
  DisplayManager displayManager;

  byte selectedChannel;
  Sequencer seq;
  
};