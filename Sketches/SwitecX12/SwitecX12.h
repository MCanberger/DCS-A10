#ifndef SwitecX12_h
#define SwitecX12_h
#include <Arduino.h>

class SwitecX12 {
  public:
    unsigned char pinStep;
    unsigned char pinDir;
    unsigned long currentStep;      // step we are currently at
    unsigned long targetStep;       // target we are moving to
    unsigned long steps;            // total steps available
    unsigned long time0;           // time when we entered this state
    unsigned int microDelay;       // microsecs until next state
    unsigned short (*accelTable)[2]; // accel table can be modified.
    unsigned int maxVel;           // fastest vel allowed
    unsigned int vel;              // steps travelled under acceleration
    char dir;                      // direction -1,0,1
    boolean stopped;               // true if stopped
    SwitecX12(unsigned long steps, unsigned char pinStep, unsigned char pinDir);

    //void stepUp();
    void step(long dir);
	void zero();
	void setZero();
	void stepTo(long position);
    void advance();
    void update();
    void setPosition(unsigned long pos);
};

#endif