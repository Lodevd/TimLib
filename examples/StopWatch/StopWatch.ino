/*
 Stop watch timer

 Start the timer at a sertain point. 
 Then read out the passed time since start. 
 You can read out the passed time as manny times as needed. 
 The timer can be restarted and stoped. 

 Connect a button between the 'BUTTON_PIN' and GND. 

 Created  4 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

#define BUTTON_PIN PIN2    // Change this to the pin you will be using. 

bool previousButtonPushed; 
bool buttonPushed();

// Create an instance of a stop watch timer.
StopWatch sw;

void setup() {
  // Input pin to trigger the timer. 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Serial to output the measured times. 
  Serial.begin(9600);

  // Start the stopwatch 
  sw.restart();

}

void loop() {
  
  bool bp = buttonPushed();

  if(previousButtonPushed & !bp){
    // Negative edge, print elapsed time since start.
    // Because there is no button debounse handeling there might be more prints then you expect.
    Serial.println(sw.watch());
    
  }
  previousButtonPushed = bp;

  // if you should want to stop te stopwatch 
  // sw.stop();

  // Or restart
  // sw.restart();

}

bool buttonPushed(){
    // When the button is pushed the pin is pulled to the GND and has value FALSE.
    return !digitalRead(BUTTON_PIN);
}