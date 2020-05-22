#define DCSBIOS_RS485_SLAVE 126
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

struct StepperConfig LeftTempConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  8, //Klar
  7, //Klar
  400,
  10
};

struct StepperConfig RightTempConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  5,
  6,
  400,
  10
};

struct StepperConfig LeftEngineFanSpeedConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  10,
  9,
  0,
  10
};

struct StepperConfig RightEngineFanSpeedConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  11,
  12,
  0,
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

SwitecX12 LeftTempMotor(LeftTempConfig.MaxSteps, LeftTempConfig.StepPin, LeftTempConfig.DirPin);
SwitecX12 RightTempMotor(RightTempConfig.MaxSteps, RightTempConfig.StepPin, RightTempConfig.DirPin);
SwitecX12 LeftEngineFanSpeedMotor(LeftEngineFanSpeedConfig.MaxSteps, LeftEngineFanSpeedConfig.StepPin, LeftEngineFanSpeedConfig.DirPin);
SwitecX12 RightEngineFanSpeedMotor(RightEngineFanSpeedConfig.MaxSteps, RightEngineFanSpeedConfig.StepPin, RightEngineFanSpeedConfig.DirPin);

X27Stepper LeftTemp(0x10b4, LeftTempMotor, LeftTempConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, LeftTempConfig.MinSteps + LeftTempConfig.MinAdjustment, LeftTempConfig.MaxSteps - LeftTempConfig.MaxAdjustment);
});

X27Stepper RightTemp(0x10b8, RightTempMotor, RightTempConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, RightTempConfig.MinSteps + RightTempConfig.MinAdjustment, RightTempConfig.MaxSteps - RightTempConfig.MaxAdjustment);
});

X27Stepper LeftEngineFanSpeed(0x10a2, LeftEngineFanSpeedMotor, LeftEngineFanSpeedConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, LeftEngineFanSpeedConfig.MinSteps + LeftEngineFanSpeedConfig.MinAdjustment, LeftEngineFanSpeedConfig.MaxSteps - LeftEngineFanSpeedConfig.MaxAdjustment);
});

X27Stepper RightEngineFanSpeed(0x10a4, RightEngineFanSpeedMotor, RightEngineFanSpeedConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, RightEngineFanSpeedConfig.MinSteps + RightEngineFanSpeedConfig.MinAdjustment, RightEngineFanSpeedConfig.MaxSteps - RightEngineFanSpeedConfig.MaxAdjustment);
});

void setup() {
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  DcsBios::setup();

}

void loop() {
  DcsBios::loop();

}
