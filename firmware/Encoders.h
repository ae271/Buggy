#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

static const float WHEEL_DIAMETER = 6.3f; // cm
static const float CIRCUMFERENCE = WHEEL_DIAMETER * PI;
static const int PULSES_PER_REVOLUTION = 4;
static const unsigned long STALE_THRESHOLD = 5000000UL; // 5 seconds
static const unsigned long SPEED_UPDATE_INTERVAL = 300000UL; // 0.3 seconds

extern volatile int leftPulsesCount;
extern volatile int rightPulsesCount;
extern volatile unsigned long leftLastPulseTime;
extern volatile unsigned long rightLastPulseTime;

void initEncoders();
void updateEncoders();
float getMeanSpeed();
float getMeanDistance();

#endif