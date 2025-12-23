#include "Motor.h"
#include <Arduino.h>

Motor::Motor() {
  pwm = 0;

  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(EN1, pwm);
  analogWrite(EN2, pwm);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
}

void Motor::moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN1, pwm);
  analogWrite(EN2, pwm);
}

void Motor::moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN1, pwm);
  analogWrite(EN2, pwm);
}

void Motor::turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN2, pwm * 0.3); 
  analogWrite(EN1, pwm);
}

void Motor::turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN1, pwm * 0.3); 
  analogWrite(EN2, pwm);
}

void Motor::setSpeed(int speedPercentage) {
  pwm = (speedPercentage != 0) ? OFF_SET + ((255 - OFF_SET) * speedPercentage / 100) : 0;
  constrain(pwm, 0, 255);
}

void Motor::stop() {
  analogWrite(EN1, LOW);
  analogWrite(EN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}