#ifndef MOTOR_H
#define MOTOR_H

// When I say left and right I am looking at the buggy from the back
const int EN1 = 9; // Left motor
const int IN1 = 8; // Left motor
const int IN2 = 7; // Left motor
const int EN2 = 10; // Right motor
const int IN3 = 11; // Right motor
const int IN4 = 19; // Right motor

class Motor {
  private:
    int pwm = 0; // range 0 - 255 basically speed
    const int OFF_SET = 90;  // Minimum speed to overcome motor inertia

  public:
    Motor();
      void moveForward();
      void moveBackward();
      void turnRight();
      void turnLeft();
      void setSpeed(int speedPercentage); // I hope the variable name is self explanatory but just incase the input should be 0 - 100 
      void stop();
};

#endif