#include <Arduino.h>
#include <WiFiS3.h>
#include <PID_v1.h>
#include "Motor.h"
#include "Infrared.h"
#include "UltrasonicSensor.h"
#include "Arduino_LED_Matrix.h"
#include "Encoders.h"

// Everything networking related
char ssid[] = "Y5";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;
WiFiServer server(5200);
WiFiClient client;
String clientCommand;

// variable names are self explanatory except "okay" i suppose 
// if okay is true the buggy can start and if its false it stops 
// basically its controlled by the start stop buttons in processing
float referenceSpeed = 0.0f;
bool objectFollowing = false;
float distanceTravelled = 0;
bool okay = false;

// initializing some objects some of them are not needed but classes make the code much easier to manage
Motor motor;
UltrasonicSensor ultrasonic;
ArduinoLEDMatrix matrix;

// PID variables for speed control
double setpoint, input, output;
double Kp = 0, Ki = 0, Kd = 0;
// PID variables forobject following
double objFollowing_setpoint = 10;
double objFollowing_input, objFollowing_output;
double objFollowing_Kp = 0, objFollowing_Ki = 0, objFollowing_Kd = 0;
// PID controllers
PID speedPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
PID objFollowing_PID(&objFollowing_input, &objFollowing_output, &objFollowing_setpoint, objFollowing_Kp, objFollowing_Ki, objFollowing_Kd, REVERSE);

void setup() {
  Serial.begin(9600);
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  matrix.begin();
  unsigned long frame[] = {0x82F44828, 0x810F1011, 0x110110F};
  matrix.loadFrame(frame);
  setupInfrared();
  initEncoders();
  speedPID.SetMode(AUTOMATIC);
  speedPID.SetOutputLimits(0, 100);
  objFollowing_PID.SetMode(AUTOMATIC);
  objFollowing_PID.SetOutputLimits(0, 100);
}

void loop() {
  updateEncoders();
  float currentSpeed = getMeanSpeed();
  float distanceTravelled = getMeanDistance();
  int distance = ultrasonic.getDistance();
  objectFollowing = distance <= 10 || distance >= 30 ? false : true;
  objFollowing_PID.SetTunings(objFollowing_Kp, objFollowing_Ki, objFollowing_Kd);
  speedPID.SetTunings(Kp, Ki, Kd);

// Here we process everything that the client sends us with the client being the processing GUI
  client = server.available();
  if (client && client.available() > 0) {
    clientCommand = client.readStringUntil('*');
    clientCommand.trim();
  }

// "g" stands for go and "s" stands for stop
  if (clientCommand == "g" || clientCommand == "s"){
    if (clientCommand == "g")
      okay = true;
    else
      okay = false;
  } 
  else if (clientCommand.charAt(0) == 'v') {
      clientCommand.remove(0, 1);
      referenceSpeed = clientCommand.toFloat();
    }
  else if (clientCommand.charAt(0) == 'p') {
    clientCommand.remove(0, 1);
    objFollowing_Kp = clientCommand.toFloat();
  }
  else if (clientCommand.charAt(0) == 'i') {
    clientCommand.remove(0, 1);
    objFollowing_Ki = clientCommand.toFloat();
  }
  else if (clientCommand.charAt(0) == 'd') {
    clientCommand.remove(0, 1);
    objFollowing_Kd = clientCommand.toFloat();
  }

// Heres the line following logic and determines which mode the buggy is in
  if (okay && distance > 10) {
    // Here the speed of the buggy is calculated using PID and depeding on whether we are 
    // object following or speed control mode the appropriate PID will be used
    if (objectFollowing) {
      objFollowing_input = distance;          
      objFollowing_PID.Compute();
      motor.setSpeed(objFollowing_output);
    } 
    else if (!objectFollowing) {
      input = currentSpeed;      
      setpoint = referenceSpeed;
      speedPID.Compute();
      motor.setSpeed(output);
    }
    // Line following logic
    if (LEYE_Current && REYE_Current) {
      motor.moveForward();
    } else if (!LEYE_Current && REYE_Current) {
      motor.turnLeft();
    } else if (LEYE_Current && !REYE_Current) {
      motor.turnRight();
    } else {
      motor.stop();
    }
  }     
  
  else if (!okay || distance <= 10) {
    motor.stop();
  }

  client.print(String(currentSpeed) + "," + String(distance) + "," + String(distanceTravelled) + "," + String(objectFollowing) + "," + "*");
  //Serial.println("Distance Travelled: " + String(distanceTravelled));
  //Serial.println("ON or OFF: " + String(okay));
  Serial.println("ClientCommand: " + clientCommand);
  //Serial.println("objectFollowing: " + String(objectFollowing));
  Serial.println("Kp: " + String(objFollowing_Kp) +" " + "Ki: " + String(objFollowing_Ki) + " " + "Kd: " + String(objFollowing_Kd) + " " + "output: " + String(objFollowing_output));
}
