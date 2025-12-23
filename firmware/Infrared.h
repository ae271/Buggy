#ifndef INFRARED_H
#define INFRARED_H
#include <Arduino.h>

// When I say left and right I am looking at the buggy from the back
const int LEYE = 2;
const int REYE = 3;
extern volatile bool LEYE_Current;
extern volatile bool REYE_Current;
extern bool LEYE_Previous;
extern bool REYE_Previous;

void setupInfrared();

#endif