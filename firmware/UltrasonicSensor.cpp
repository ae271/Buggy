#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor() {
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);
}

int UltrasonicSensor::getDistance() {
  digitalWrite(US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);

  int duration = pulseIn(US_ECHO, HIGH);
  return duration / 58;
}

void UltrasonicSensor::printDistance(int distance) {
  Serial.print("Distance detected: ");
  Serial.print(distance);
  Serial.println(" cm");
}