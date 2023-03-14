#include <Arduino.h>
#include "Rotarydrive.h"
#include "Constants.h"
#include "StepperControl.h"

enum Mode
{
    adjustingJoint1,
    adjustingJoint2,
    adjustingJoint3,
    adjustingJoint4,
};

Mode currentMode = adjustingJoint1;

Rotarydrive Rotary1(ROTARY_ENC_1_A, ROTARY_ENC_1_B);
StepperControl MOTOR1(STEPPER1_DIR_PIN, STEPPER1_STEP_PIN);
StepperControl MOTOR2(STEPPER2_DIR_PIN, STEPPER2_STEP_PIN);
StepperControl MOTOR3(STEPPER3_DIR_PIN, STEPPER3_STEP_PIN);
StepperControl MOTOR4(STEPPER4_DIR_PIN, STEPPER4_STEP_PIN);

int buttonCoolDowncounterer = 0;
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile SemaphoreHandle_t timerSemaphore;

void handleModeChange(Mode newMode)
{
    if (buttonCoolDowncounterer < BUTTON_COOLDOWN_CYCLES)
    {
        buttonCoolDowncounterer++;
    }
    if (digitalRead(BUTTON_PIN) && buttonCoolDowncounterer >= BUTTON_COOLDOWN_CYCLES)
    {
        buttonCoolDowncounterer = 0;
        currentMode = newMode;
    }
}

void IRAM_ATTR onTimer()
{
    digitalWrite(EXECUTING_ISR_CODE, HIGH);

    switch (currentMode)
    {
    case adjustingJoint1:
        {
          Rotary1.update();
          MOTOR1.RUN(Rotary1.counter);
        
          handleModeChange(adjustingJoint2);
        }
        break;
    case adjustingJoint2:
        {
          Rotary1.update();
          MOTOR2.RUN(Rotary1.counter);
          
          handleModeChange(adjustingJoint3);
                
        }
         break;
    case adjustingJoint3:
        {        
          Rotary1.update();
          MOTOR3.RUN(Rotary1.counter);
          
          handleModeChange(adjustingJoint4);
        }
        break;
    case adjustingJoint4:
        {
           Rotary1.update();
           MOTOR4.RUN(Rotary1.counter);
          
           
        }
        break;
    
    default:
        break;
    }

    xSemaphoreGiveFromISR(timerSemaphore, NULL);
    digitalWrite(EXECUTING_ISR_CODE, LOW);
}

void setup()
{
    Serial.begin(115200);
     pinMode(EXECUTING_ISR_CODE, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    timerSemaphore = xSemaphoreCreateBinary();

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    // onTimer gets called every 100uS.
    timerAlarmWrite(timer, 100, true);
    timerAlarmEnable(timer);


}

void loop()
{
        if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
    {
      

        portENTER_CRITICAL(&timerMux);
      
        portEXIT_CRITICAL(&timerMux);

        //Serial.print("MODE: ");
        //Serial.print(currentMode);
        delay(10);
    }
}
