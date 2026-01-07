/*
 Pause timer

 The timer runs while the run trigger is high. 
 When the trigger is false the timer pauses and continues when the trigger is high again. 
 When the timer reaches the setpoint it stops timing and returns true. 
 The timer keeps returning true until it receives a reset command. 

 Connect a button between the 'BUTTON_PIN' and GND. 

 Created  7 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

#define BUTTON_PIN PIN2    // Change this to the pin you will be using. 

bool buttonPushed();

// Create an instance of a stop watch timer.
PauseTimer pt(10000);      // Setpoint of 10 seconds

void setup() {
  // Input pin to trigger the timer. 
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Serial to output the measured times. 
  Serial.begin(9600);

}

void loop() {

  if(pt.run(buttonPushed())){
    Serial.println("You pushed the button for a total time of 10 seconds.");
    pt.reset();
  }

  // If your setpoint is dynamic you can change the code to
  // uint32_t setpoint;
  // if(pt.run(buttonPushed(), setpoint)){

}

bool buttonPushed(){
    // When the button is pushed the pin is pulled to the GND and has value FALSE.
    return !digitalRead(BUTTON_PIN);
}