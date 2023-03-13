#include <Arduino.h>
#include "Rotarydrive.h"
#include "Constants.h"

enum Mode
{
    adjustingJoint1,
    adjustingJoint2,
    adjustingJoint3,
    adjustingJoint4,
    doingControlledMovements,
    error
};

Mode currentMode = adjustingJoint1;

Rotarydrive Rotary1(ROTARY_ENC_1_A, ROTARY_ENC_1_B);

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
        Rotary1.update();
        digitalWrite(STEPPER1_DIR_PIN, Rotary1.currentRot);
        digitalWrite(STEPPER1_STEP_PIN, Rotary1.counter % 2);

        handleModeChange(adjustingJoint2);
        break;
    case adjustingJoint2:
        Rotary1.update();
        digitalWrite(STEPPER2_DIR_PIN, Rotary1.currentRot);
        digitalWrite(STEPPER2_STEP_PIN, Rotary1.counter % 2);

        handleModeChange(adjustingJoint3);
        break;
    case adjustingJoint3:
        Rotary1.update();
        digitalWrite(STEPPER3_DIR_PIN, Rotary1.currentRot);
        digitalWrite(STEPPER3_STEP_PIN, Rotary1.counter % 2);

        handleModeChange(adjustingJoint4);
        break;
    case adjustingJoint4:
        Rotary1.update();
        digitalWrite(STEPPER4_DIR_PIN, Rotary1.currentRot);
        digitalWrite(STEPPER4_STEP_PIN, Rotary1.counter % 2);

        handleModeChange(doingControlledMovements);
        break;
    case doingControlledMovements:
        portENTER_CRITICAL_ISR(&timerMux);
        portEXIT_CRITICAL_ISR(&timerMux);
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
    // Set 80 divider for prescaler
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
        Serial.print(currentMode);
        //Serial.print("pos: ");
        //Serial.println(pos);
        delay(10);
    }
}

