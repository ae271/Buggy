#include "Infrared.h"

volatile bool LEYE_Current = true;
volatile bool REYE_Current = true;
bool LEYE_Previous = true;
bool REYE_Previous = true;

void LEYE_isr() {
  LEYE_Current = digitalRead(LEYE);
}

void REYE_isr() {
  REYE_Current = digitalRead(REYE);
}

void setupInfrared() {
  pinMode(LEYE, INPUT);
  pinMode(REYE, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(LEYE), LEYE_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(REYE), REYE_isr, CHANGE);
  
  LEYE_Previous = digitalRead(LEYE);
  REYE_Previous = digitalRead(REYE);
}