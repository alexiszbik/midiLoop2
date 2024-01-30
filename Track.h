
#define STEP_LENGTH 6

class Track {

public:
  void tick() {
    
    currentTick++;
    if (currentTick % STEP_LENGTH == 0) {
      currentStep++;
      if (currentStep >= stepsPerBar) {
        currentStep = 0;
        currentBar++;
        if (currentBar >= barCount) {
          currentBar = 0;
        }
      }
    }

    currentTick = currentTick % 96;
  }

  void resetTicks() {
    currentTick = 0;
    currentStep = 0;
    currentBar = 0;
  }

public:
  byte barCount = 4;
  byte stepsPerBar = 16;

  byte currentTick = 0;
  byte currentStep = 0;
  byte currentBar = 0;
  
};