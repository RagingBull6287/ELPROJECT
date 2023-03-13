#ifndef Rotarydrive_h
#define Rotarydrive_h
#include "Arduino.h"
#define DIR_CCW 0x10
#define DIR_CW 0x20
volatile unsigned char state = 0;


class Rotarydrive
{
  public:
    Rotarydrive(int pinSignalA, int pinSignalB);
    void update();
    int counter;
    int result;
    int currentRot;
    // currentRot:
    // 0 -> rotating CCW
    // 1 -> rotating CW
  private:
    int _pinSignalA;
    int _pinSignalB;
    int _state;
  
};

#endif