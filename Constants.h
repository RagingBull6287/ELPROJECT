#ifndef Constants_h
#define Constants_h
#include "Arduino.h"

#define STEPPER1_STEP_PIN 19
#define STEPPER1_DIR_PIN 18
#define STEPPER2_STEP_PIN 22
#define STEPPER2_DIR_PIN 21
#define STEPPER3_STEP_PIN 14
#define STEPPER3_DIR_PIN 12
#define STEPPER4_STEP_PIN 26
#define STEPPER4_DIR_PIN 27
#define STEPPER5_STEP_PIN 2
#define STEPPER5_DIR_PIN 15

#define BUTTON_PIN 34
#define NAN_ALERT_LED 4 // This LEDs sole purpose is to alarm the user that there was NaN result in the IK calculations.
#define BUTTON_COOLDOWN_CYCLES 5000

#define EXECUTING_ISR_CODE 23
#define ROTARY_ENC_1_A 33
#define ROTARY_ENC_1_B 32

#define PULSES_PER_REVOLUTION 2048

// NOTE: SineStepper and MoveBatch ids must be lower then MAX_NUM_OF_STEPPERS
#define MAX_NUM_OF_STEPPERS 10
#define MAX_NUM_OF_BATCHED_MOVES 100
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

#endif
