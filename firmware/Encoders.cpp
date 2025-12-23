#include "Encoders.h"

volatile int leftPulsesCount = 0;
volatile int rightPulsesCount = 0;
volatile unsigned long leftLastPulseTime = 0;
volatile unsigned long rightLastPulseTime = 0;
static int prevLeftPulsesCount = 0;
static int prevRightPulsesCount = 0;
static unsigned long lastSpeedComputeTime = 0;
static float leftWheelDistance = 0.0f;
static float rightWheelDistance = 0.0f;
static float leftWheelSpeed = 0.0f;
static float rightWheelSpeed = 0.0f;
static float meanDistanceVal = 0.0f;
static float meanSpeedVal = 0.0f;

static void leftEncoderISR() {
  leftPulsesCount++;
  leftLastPulseTime = micros();
}

static void rightEncoderISR() {
  rightPulsesCount++;
  rightLastPulseTime = micros();
}

void initEncoders() {
  pinMode(17, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(17), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(12), rightEncoderISR, RISING);
  leftLastPulseTime = micros();
  rightLastPulseTime = micros();
  lastSpeedComputeTime = micros();
}

void updateEncoders() {
  unsigned long now = micros();
  if (now - lastSpeedComputeTime >= SPEED_UPDATE_INTERVAL) {
    int leftDelta = leftPulsesCount - prevLeftPulsesCount;
    int rightDelta = rightPulsesCount - prevRightPulsesCount;
    float leftDist = (leftDelta * CIRCUMFERENCE) / PULSES_PER_REVOLUTION;
    float rightDist = (rightDelta * CIRCUMFERENCE) / PULSES_PER_REVOLUTION;
    float dt = (now - lastSpeedComputeTime) * 1e-6;
    if (dt > 0) {
      leftWheelSpeed = leftDist / dt;
      rightWheelSpeed = rightDist / dt;
    }
    leftWheelDistance += leftDist;
    rightWheelDistance += rightDist;
    meanDistanceVal = (leftWheelDistance + rightWheelDistance) * 0.5f;
    meanSpeedVal = (leftWheelSpeed + rightWheelSpeed) * 0.5f;
    prevLeftPulsesCount = leftPulsesCount;
    prevRightPulsesCount = rightPulsesCount;
    lastSpeedComputeTime = now;
  }
  if ((now - leftLastPulseTime) > STALE_THRESHOLD) {
    leftWheelSpeed = 0.0f;
  }
  if ((now - rightLastPulseTime) > STALE_THRESHOLD) {
    rightWheelSpeed = 0.0f;
  }
  meanSpeedVal = (leftWheelSpeed + rightWheelSpeed) * 0.5f;
}

float getMeanSpeed() {
  return meanSpeedVal;
}

float getMeanDistance() {
  return meanDistanceVal;
}