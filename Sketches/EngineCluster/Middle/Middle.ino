#define DCSBIOS_RS485_SLAVE 127
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

struct StepperConfig LeftRPMConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  8, //Klar
  7, //Klar
  340,
  10
};

struct StepperConfig RightRPMConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  5,
  6,
  320,
  10
};

struct StepperConfig LeftFuelFlowConfig = {
  3780, //315 degrees * 3 steps * 4 microsteps;
  0,
  10,
  9,
  0,
  10
};

struct StepperConfig RightFuelFlowConfig = {
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

SwitecX12 LeftRPMMotor(LeftRPMConfig.MaxSteps, LeftRPMConfig.StepPin, LeftRPMConfig.DirPin);
SwitecX12 RightRPMMotor(RightRPMConfig.MaxSteps, RightRPMConfig.StepPin, RightRPMConfig.DirPin);
SwitecX12 LeftFuelFlowMotor(LeftFuelFlowConfig.MaxSteps, LeftFuelFlowConfig.StepPin, LeftFuelFlowConfig.DirPin);
SwitecX12 RightFuelFlowMotor(RightFuelFlowConfig.MaxSteps, RightFuelFlowConfig.StepPin, RightFuelFlowConfig.DirPin);

X27Stepper LeftRPM(0x10a8, LeftRPMMotor, LeftRPMConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, LeftRPMConfig.MinSteps + LeftRPMConfig.MinAdjustment, LeftRPMConfig.MaxSteps - LeftRPMConfig.MaxAdjustment);
});

X27Stepper RightRPM(0x10ac, RightRPMMotor, RightRPMConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, RightRPMConfig.MinSteps + RightRPMConfig.MinAdjustment, RightRPMConfig.MaxSteps - RightRPMConfig.MaxAdjustment);
});

X27Stepper LeftFuelFlow(0x10ae, LeftFuelFlowMotor, LeftFuelFlowConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, LeftFuelFlowConfig.MinSteps + LeftFuelFlowConfig.MinAdjustment, LeftFuelFlowConfig.MaxSteps - LeftFuelFlowConfig.MaxAdjustment);
});

X27Stepper RightFuelFlow(0x10b0, RightFuelFlowMotor, RightFuelFlowConfig, [](unsigned int newValue) -> unsigned int {
  return map(newValue, 0, 65535, RightFuelFlowConfig.MinSteps + RightFuelFlowConfig.MinAdjustment, RightFuelFlowConfig.MaxSteps - RightFuelFlowConfig.MaxAdjustment);
});

void setup() {
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  DcsBios::setup();

}

void loop() {
  DcsBios::loop();

}
