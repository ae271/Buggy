import controlP5.*;
import processing.net.*;

ControlP5 cp5;
Slider slider;
Button startButton;
Button stopButton;
Client client;
int objectTrackingMode = 0;
float objectDistance = 0;
float referenceSpeed = 50;  // Desired speed in cm/s
float prevReferenceSpeed = 0;
float currentSpeed = 0; // Actual speed from encoders
float distanceTravelled = 0;
float Kp = 0;
float Ki = 0;
float Kd = 0;
float prevKp = 0;
float prevKi = 0;
float prevKd = 0;

// New vertical sliders
Slider KpSlider, KiSlider, KdSlider;
PImage img;

void setup() {
  size(900, 600);
  surface.setResizable(true);
  //fullScreen();
  background(255);
  img = loadImage("Hello_Kitty.png");
  img.resize(900, 600);
 
  cp5 = new ControlP5(this);
 
  // Initial scaling factors based on original design dimensions
  float scaleX = width / 900.0;
  float scaleY = height / 600.0;
  
  // Create the horizontal speed slider
  slider = cp5.addSlider("speed cm/s")
              .setPosition(width/2 - int(200 * scaleX), height/2 + int(175 * scaleY))
              .setSize(int(400 * scaleX), int(40 * scaleY))
              .setRange(0, 25)
              .setValue(10)
              .setNumberOfTickMarks(26)
              .snapToTickMarks(true);
  slider.setColorForeground(color(255, 105, 180));
  slider.setColorBackground(color(255, 182, 193));
  slider.setColorActive(color(255, 20, 147));
 
  // Create three vertical sliders with the same color theme,
  // arranged on the left side.
  KpSlider = cp5.addSlider("KpSlider")
                .setPosition(int(50 * scaleX), int(100 * scaleY))
                .setSize(int(40 * scaleX), int(120 * scaleY))
                .setRange(0, 20)
                .setValue(0)
                .setNumberOfTickMarks(101)
                .snapToTickMarks(true)
                .setSliderMode(Slider.VERTICAL);
  KpSlider.setColorForeground(color(255, 105, 180));
  KpSlider.setColorBackground(color(255, 182, 193));
  KpSlider.setColorActive(color(255, 20, 147));
  
  KiSlider = cp5.addSlider("KiSlider")
                .setPosition(int(50 * scaleX), int(100 * scaleY) + int(140 * scaleY))
                .setSize(int(40 * scaleX), int(120 * scaleY))
                .setRange(0, 5)
                .setValue(0)
                .setNumberOfTickMarks(101)
                .snapToTickMarks(true)
                .setSliderMode(Slider.VERTICAL);
  KiSlider.setColorForeground(color(255, 105, 180));
  KiSlider.setColorBackground(color(255, 182, 193));
  KiSlider.setColorActive(color(255, 20, 147));
  
  KdSlider = cp5.addSlider("KdSlider")
                .setPosition(int(50 * scaleX), int(100 * scaleY) + int(280 * scaleY))
                .setSize(int(40 * scaleX), int(120 * scaleY))
                .setRange(0, 5)
                .setValue(0)
                .setNumberOfTickMarks(101)
                .snapToTickMarks(true)
                .setSliderMode(Slider.VERTICAL);
  KdSlider.setColorForeground(color(255, 105, 180));
  KdSlider.setColorBackground(color(255, 182, 193));
  KdSlider.setColorActive(color(255, 20, 147));
 
  client = new Client(this, "192.168.214.217", 5200);
 
  // Create the Start button
  startButton = cp5.addButton("Start")
     .setPosition(width/2 - int(120 * scaleX), int(575 * scaleY))
     .setSize(int(120 * scaleX), int(60 * scaleY))
     .setColorBackground(color(0, 255, 0))
     .onPress(new CallbackListener() {
       public void controlEvent(CallbackEvent event) {
         startButton.setColorBackground(color(128, 128, 128));
         stopButton.setColorBackground(color(255, 0, 0));
         sendCommand("g");
       }
     });
 
  // Create the Stop button
  stopButton = cp5.addButton("Stop")
     .setPosition(width/2 + int(20 * scaleX), int(575 * scaleY))
     .setSize(int(120 * scaleX), int(60 * scaleY))
     .setColorBackground(color(255, 0, 0))
     .onPress(new CallbackListener() {
       public void controlEvent(CallbackEvent event) {
         sendCommand("s");
         stopButton.setColorBackground(color(128, 128, 128));
         startButton.setColorBackground(color(0, 255, 0));
       }
     });
}

void draw() {
  // Calculate scaling factors relative to the original 900x600 design
  float scaleX = width / 900.0;
  float scaleY = height / 600.0;
  
  background(255, 216, 216);
  image(img, 0, 0);
 
  // Draw the main container rectangle with scaled position and size
  fill(255, 182, 193, 180);
  stroke(255, 105, 180);
  strokeWeight(3);
  rect(200 * scaleX, 50 * scaleY, 500 * scaleX, 450 * scaleY, 20 * scaleX);
 
  // Draw header text and line with scaled sizes
  fill(255, 20, 147);
  textSize(28 * scaleY);
  textAlign(CENTER);
  text("Y05 Buggy Control", width/2, 100 * scaleY);
  stroke(255, 20, 147);
  line(width/2 - 100 * scaleX, 110 * scaleY, width/2 + 100 * scaleX, 110 * scaleY);
 
  // Draw telemetry display boxes
  textSize(20 * scaleY);
  fill(255, 105, 180, 100);
  rect(300 * scaleX, 150 * scaleY, 300 * scaleX, 40 * scaleY, 10 * scaleX);
  rect(300 * scaleX, 200 * scaleY, 300 * scaleX, 40 * scaleY, 10 * scaleX);
  rect(300 * scaleX, 250 * scaleY, 300 * scaleX, 40 * scaleY, 10 * scaleX);
  rect(300 * scaleX, 300 * scaleY, 300 * scaleX, 40 * scaleY, 10 * scaleX);
  rect(300 * scaleX, 350 * scaleY, 300 * scaleX, 40 * scaleY, 10 * scaleX);
 
  // Draw telemetry text with updated positions
  fill(255);
  textAlign(CENTER, CENTER);
  text("Control Mode: " + (objectTrackingMode == 1 ? "Object Tracking" : "Reference Speed"), 450 * scaleX, 170 * scaleY);
  text("Reference Speed: " + referenceSpeed + " cm/s", 450 * scaleX, 220 * scaleY);
  text("Current Speed: " + currentSpeed + " cm/s", 450 * scaleX, 270 * scaleY);
  text("Object Distance: " + objectDistance + " cm", 450 * scaleX, 320 * scaleY);
  text("Distance travelled: " + distanceTravelled + " cm", 450 * scaleX, 370 * scaleY);
  
  // Update the positions and sizes of the CP5 elements to match the new window size
  slider.setPosition(width/2 - int(200 * scaleX), height/2 + int(125 * scaleY));
  slider.setSize(int(400 * scaleX), int(40 * scaleY));
  
  startButton.setPosition(width/2 - int(120 * scaleX), int(525 * scaleY));
  startButton.setSize(int(120 * scaleX), int(60 * scaleY));
  
  stopButton.setPosition(width/2 + int(20 * scaleX), int(525 * scaleY));
  stopButton.setSize(int(120 * scaleX), int(60 * scaleY));
  
  // Update vertical sliders positions and sizes for responsive design
  KpSlider.setPosition(int(50 * scaleX), int(100 * scaleY));
  KpSlider.setSize(int(40 * scaleX), int(120 * scaleY));
  
  KiSlider.setPosition(int(50 * scaleX), int(100 * scaleY) + int(140 * scaleY));
  KiSlider.setSize(int(40 * scaleX), int(120 * scaleY));
  
  KdSlider.setPosition(int(50 * scaleX), int(100 * scaleY) + int(280 * scaleY));
  KdSlider.setSize(int(40 * scaleX), int(120 * scaleY));
  
  receiveTelemetry(client.readStringUntil('*'));
  println(objectTrackingMode);
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(slider)) {
    referenceSpeed = theEvent.getValue();
    if (prevReferenceSpeed != referenceSpeed) {
      sendCommand("v" + str(referenceSpeed));
      prevReferenceSpeed = referenceSpeed;
    }
  }
  else if (theEvent.isFrom(KpSlider)) {
    Kp = theEvent.getValue();
    if (prevKp != Kp) {
      sendCommand("p" + str(Kp));
      prevKp = Kp;
    }
  }
    else if (theEvent.isFrom(KiSlider)) {
    Ki = theEvent.getValue();
    if (prevKi != Ki) {
      sendCommand("i" + str(Ki));
      prevKi = Ki;
    }
  }
    else if (theEvent.isFrom(KdSlider)) {
    Kd = theEvent.getValue();
    if (prevKd != Kd) {
      sendCommand("d" + str(Kd));
      prevKd = Kd;
    }
  }
}

void sendCommand(String command) {
  if (client.active()) {
    client.write(command + "*");
    println(command);
  } else {
    println("Arduino client not connected");
  }    
}

void receiveTelemetry(String data) {
  if (data == null) {
    return;
  }
  String[] parts = split(data, ",");
  if (parts.length >= 4) {
    currentSpeed = float(parts[0]); 
    objectDistance = float(parts[1]);
    distanceTravelled = float(parts[2]);
    objectTrackingMode = int(parts[3]);
  }
}
