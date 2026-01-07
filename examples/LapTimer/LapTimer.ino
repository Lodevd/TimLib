/*
 Lap timer

 This returns te passed time since the last call. 
 For example to analyze code execution times. 

 Connect a button between the 'BUTTON_PIN' and GND. 

 Created  4 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

#define BUTTON_PIN PIN2    // Change this to the pin you will be using. 

bool previousButtonPushed; 
bool buttonPushed();

// Create an instance of a lap timer.
LapTimer lt;

void setup() {
  // Input pin to trigger the timer. 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Serial to output the measured times. 
  Serial.begin(9600);
}

void loop() {
  
  // Prints the time between button pushes.
  // Because there is no button debounse handeling there might be more prints then you expect.
  // First push will print value 0 because there is no reference time yet. 
  bool bp = buttonPushed();
  if(!previousButtonPushed & bp){
    Serial.println(lt.lap());
  }
  previousButtonPushed = bp;

}

bool buttonPushed(){
    // When the button is pushed the pin is pulled to the GND and has value FALSE.
    return !digitalRead(BUTTON_PIN);
}