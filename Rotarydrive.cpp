#include <Arduino.h>
#include "Rotarydrive.h"

Rotarydrive::Rotarydrive(int pinSignalA, int pinSignalB)
{
    _pinSignalA = pinSignalA;
    _pinSignalB = pinSignalB;
    pinMode(_pinSignalA, INPUT);
    pinMode(_pinSignalB, INPUT);
    counter = 0;
    _state = 0;
}

void Rotarydrive::update()
{
   const unsigned char ttable[7][4] = {
{0x0, 0x2, 0x4, 0x0}, {0x3, 0x0, 0x1, 0x10},
{0x3, 0x2, 0x0, 0x0}, {0x3, 0x2, 0x1, 0x0},
{0x6, 0x0, 0x4, 0x0}, {0x6, 0x5, 0x0, 0x20},
{0x6, 0x5, 0x4, 0x0},
};

int counter;
 unsigned char result;
  /* Reset the counter */
 counter = 0;

  while(1)
  {
    /* Read the status of the dial */
    unsigned char pinstate = (digitalRead(_pinSignalA) << 1) | digitalRead(_pinSignalB);
    state = ttable[state & 0xf][pinstate];
    result=state&0x30;
    if(result==DIR_CCW){
      counter++;
      currentRot =0;
    } 
        if(result==DIR_CW)
        {
      counter--;
      currentRot = 1;
    }  
  }
}