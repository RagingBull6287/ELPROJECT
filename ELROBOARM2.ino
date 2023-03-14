#include <Arduino.h>
#include "Rotarydrive.h"
#include "Constants.h"
#include "StepperControl.h"


Rotarydrive Rotary1(ROTARY_ENC_1_A, ROTARY_ENC_1_B);
StepperControl MOTOR1(STEPPER1_DIR_PIN, STEPPER1_STEP_PIN);
StepperControl MOTOR2(STEPPER2_DIR_PIN, STEPPER2_STEP_PIN);
StepperControl MOTOR3(STEPPER3_DIR_PIN, STEPPER3_STEP_PIN);
StepperControl MOTOR4(STEPPER4_DIR_PIN, STEPPER4_STEP_PIN);
StepperControl MOTOR5(STEPPER5_DIR_PIN, STEPPER5_STEP_PIN);
struct Button {
    uint32_t numberKeyPresses;
    bool pressed;
};
Button button1 = {0, false};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 



void IRAM_ATTR isr() {
    button_time = millis();
if (button_time - last_button_time > 250)
{
  {
       if(button1.numberKeyPresses == 4)
       {
         button1.numberKeyPresses = 0;
       }
       else
        button1.numberKeyPresses++;
  }
        button1.pressed = true;
       last_button_time = button_time;
}
}


void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, isr, FALLING);
}

void loop()
{
 if (button1.pressed) {
        Serial.printf("Button has been pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;

    }
 

  if(button1.numberKeyPresses == 0)
        {
          Rotary1.update();
          MOTOR1.RUN(Rotary1.counter);
          Serial.print("Adjusting Joint 1");
        }
        
  else if(button1.numberKeyPresses == 1 )
        {
          Rotary1.update();
          MOTOR2.RUN(Rotary1.counter);
          Serial.print("Adjusting Joint 2");
                
        }
         
   else if(button1.numberKeyPresses == 2 )
        {        
          Rotary1.update();
          MOTOR3.RUN(Rotary1.counter);
          Serial.print("Adjusting Joint 3");
        }
        
    else if(button1.numberKeyPresses == 3)
        {
           Rotary1.update();
           MOTOR4.RUN(Rotary1.counter);
          Serial.print("Adjusting Joint 4");
           
        }
    else if(button1.numberKeyPresses == 4)
        {
           Rotary1.update();
           MOTOR5.RUN(Rotary1.counter);
          Serial.print("Adjusting Base");
        }
    else
         Serial.println("error");
}
