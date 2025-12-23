#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include <Arduino.h>

const int US_TRIG = 5;
const int US_ECHO = 6;

class UltrasonicSensor {
  public:
    UltrasonicSensor();
    int getDistance();
    void printDistance(int distance);
};

#endif