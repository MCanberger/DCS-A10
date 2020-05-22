#define DCSBIOS_RS485_SLAVE 128
//#define DCSBIOS_IRQ_SERIAL
#define TXENABLE_PIN 2

#include "SwitecX12.h"
#include "DcsBios.h"

const int RESET = 12;

const bool InitToMax = true;

struct StepperConfig {
  unsigned int MaxSteps;
  unsigned int MinSteps;
  unsigned int StepPin;
  unsigned int DirPin;
  unsigned int MaxAdjustment;
  unsigned int MinAdjustment; 
};

struct StepperConfig LeftPSIConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  8, //Klar
  7, //Klar
  600,
  10
};

struct StepperConfig RightPSIConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  5,
  6,
  600,
  10
};

struct StepperConfig APURPMConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  10,
  9,
  870,
  10
};

struct StepperConfig APUTempConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  11,
  12,
  600,
  10
};

class X27Stepper : public DcsBios::Int16Buffer {
  private:
    SwitecX12& stepper;
    StepperConfig& stepperConfig;
    unsigned int (*map_function)(unsigned int);
    unsigned char initState;
  public:
    X27Stepper(unsigned int address, SwitecX12& stepper, StepperConfig& stepperConfig, unsigned int (*map_function)(unsigned int))
    : Int16Buffer(address), stepper(stepper), stepperConfig(stepperConfig), map_function(map_function), initState(0) {
    }
    
    virtual void loop() {
      if (initState == 0) { // not initialized yet
        digitalWrite(RESET, HIGH);
        stepper.zero();
        initState = 1;
      }
      if (initState == 1) { // zeroing
        if (stepper.stopped) {
          if (InitToMax)
            stepper.setPosition(stepperConfig.MaxSteps - stepperConfig.MaxAdjustment);
          else
            stepper.setPosition(stepperConfig.MinSteps + stepperConfig.MinAdjustment);     
          stepper.update();
          initState = 2;
        }
      }
      if (initState == 2) { // running normally
        if (hasUpdatedData()) {
          unsigned int newPosition = map_function(getData());
          newPosition = constrain(newPosition, stepperConfig.MinSteps, stepperConfig.MaxSteps);
          stepper.setPosition(newPosition);
        }
        stepper.update();
      }
    }
};

SwitecX12 LeftPSIMotor(LeftPSIConfig.MaxSteps, LeftPSIConfig.StepPin, LeftPSIConfig.DirPin);
SwitecX12 RightPSIMotor(RightPSIConfig.MaxSteps, RightPSIConfig.StepPin, RightPSIConfig.DirPin);
SwitecX12 APURPMMotor(APURPMConfig.MaxSteps, APURPMConfig.StepPin, APURPMConfig.DirPin);
SwitecX12 APUTempMotor(APUTempConfig.MaxSteps, APUTempConfig.StepPin, APUTempConfig.DirPin);

X27Stepper LeftPSI(0x10c6, LeftPSIMotor, LeftPSIConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, LeftPSIConfig.MinSteps + LeftPSIConfig.MinAdjustment, LeftPSIConfig.MaxSteps - LeftPSIConfig.MaxAdjustment);
});

X27Stepper RightPSI(0x10c8, RightPSIMotor, RightPSIConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, RightPSIConfig.MinSteps + RightPSIConfig.MinAdjustment, RightPSIConfig.MaxSteps - RightPSIConfig.MaxAdjustment);
});

X27Stepper APURPM(0x10be, APURPMMotor, APURPMConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, APURPMConfig.MinSteps + APURPMConfig.MinAdjustment, APURPMConfig.MaxSteps - APURPMConfig.MaxAdjustment);
});

X27Stepper APUTemp(0x10c0, APUTempMotor, APUTempConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, APUTempConfig.MinSteps + APUTempConfig.MinAdjustment, APUTempConfig.MaxSteps - APUTempConfig.MaxAdjustment);
});

void setup() {
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  DcsBios::setup();

}

void loop() {
  DcsBios::loop();

}
