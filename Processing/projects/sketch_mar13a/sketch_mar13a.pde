/*
  Serial Graphing Sketch
  by Tom Igoe
  Language: Processing
 
  This sketch takes ASCII values from the serial port 
  at 9600 bps and graphs them.
  The values should be comma-delimited, with a newline 
  at the end of every set of values.
  The expected range of the values is between 0 and 1023.
 
  Created 20 April 2005
  Updated 27 June 2008
*/
 
import processing.serial.*;
 
int maxNumberOfSensors = 6;       // Arduino has 6 analog inputs, so I chose 6
//int[] maxSensorValue = {255,255,255,65535,65535,65535,65535,65535,65535};
//int[] minSensorValue = {0,0,0,0,0,0,0,0,0};
//int[] maxSensorValue = {512,512,512,655360,20000,20000,20000};
//int[] minSensorValue = {-512,-512,-512,0,-20000,-20000,-20000};
//int[] maxSensorValue = {512,512,512};              // SINGLE OUTPUT: ACCELEROMETER
//int[] minSensorValue = {-512,-512,-512};           // SINGLE OUTPUT: ACCELEROMETER
//int[] maxSensorValue = {65535,65535,65535,65535};          // SINGLE OUTPUT: GYROSCOPE
//int[] minSensorValue = {-65535,-65535,-65535,-65535};       // SINGLE OUTPUT: GYROSCOPE

//int[] maxSensorValue = {360,360};
//int[] minSensorValue = {0,0};

int[] maxSensorValue = {90,90,90,90,90,90};
int[] minSensorValue = {-90,-90,-90,-90,-90,-90};


boolean fontInitialized = false;  // whether the font's been initialized
Serial myPort;                    // The serial port
 
float[] previousValue = new float[maxNumberOfSensors];  // array of previous values
int xpos = 0;                     // x position of the graph
PFont myFont;                     // font for writing text to the window
 
void setup () {
  // set up the window to whatever size you want:
  size(1900, 1000);        
  println("HELLO");
  // List all the available serial ports:
  println(Serial.list());
  println("WTFFFFFFFFFFF");
  
  // I know that the first port in the serial list on my mac
  // is always my  Arduino or Wiring module, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  
  //println(Serial.list()[0]);
  
  //String portName = Serial.list()[0];
  //myPort = new Serial(this, portName, 115200);
  myPort = new Serial(this, "/dev/ttyACM0", 115200); 
  myPort.clear();
  // don't generate a serialEvent() until you get a newline (\n) byte:
  myPort.bufferUntil('\n');
  // create a font with the fourth font available to the system:
  myFont = createFont(PFont.list()[3], 14);
  textFont(myFont);
  fontInitialized = true;
  // set inital background:
  background(0);
  // turn on antialiasing:
  smooth();
}


void draw () {
  // nothing happens in the draw loop, 
  // but it's needed to keep the program running
}
 
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
 
  // if it's not empty:
  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
 
    // convert to an array of ints:
    int incomingValues[] = int(split(inString, ","));
 
    // print out the values
    //  print("length: " + incomingValues.length + " values.\t");
    if (incomingValues.length <= maxNumberOfSensors && incomingValues.length > 0) {
      for (int i = 0; i < incomingValues.length; i++) {
 
        // map the incoming values (0 to  1023) to an appropriate
        // graphing range (0 to window height/number of values):
          float ypos = map(incomingValues[i], minSensorValue[i], maxSensorValue[i], 0, height/incomingValues.length);
        // figure out the y position for this particular graph:
        float graphBottom = i * height/incomingValues.length;
        ypos = ypos + graphBottom;
 
        // make a black block to erase the previous text:
        noStroke();
        fill(0);
        rect(10, graphBottom+1, 110, 20);
 
        // print the sensor numbers to the screen:
        fill(255);
        int textPos = int(graphBottom) + 14;
        // sometimes serialEvent() can happen before setup() is done.
        // so you need to make sure the font is initialized before
        // you text():
        if (fontInitialized) {
          text("Sensor " + i + ":" + incomingValues[i], 10, textPos);
        }
        // draw a line at the bottom of each graph:
        stroke(127);
        line(0, graphBottom, width, graphBottom);
        // change colors to draw the graph line:
        stroke(64*i, 32*i, 255);
        line(xpos, previousValue[i], xpos+1, ypos);
        // save the current value to be the next time's previous value:
        previousValue[i] = ypos;
      }
    }
    // if you've drawn to the edge of the window, start at the beginning again:
    if (xpos >= width) {
      xpos = 0;
      background(0);
    } 
    else {
      xpos++;
    }
  }
}

