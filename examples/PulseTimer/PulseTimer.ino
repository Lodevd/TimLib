/*
 Pulse timer

 The timer will give a fixed length pulse starting on the positive edge of the trigger signal. 
 The input signal can be longer or shorter then the pulse time. 
 A second positive edge while the pulse is active will not increase the pulse length.  

 This example is for the aruino uno or other bords with a build in led. 'LED_BUILTIN'
 Connect a button between the 'BUTTON_PIN' and GND. 

 Created  4 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

#define BUTTON_PIN PIN2    // Change this to the pin you will be using. 

bool buttonPushed();

// Create an instance of an pulse timer, with 5000ms delay. 
PulseTimer  pt(5000);     // Pulse time setpoint set to 5000ms.
// PulseTimer  pt;        // No initial setpoint. 

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // input pin to trigger the timer. 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  
    // Independant of how long the button is pushed, the led will be on for 5 seconds;
    digitalWrite(LED_BUILTIN, pt.run(buttonPushed()));

    // You can also specify the pulse time while calling run. 
    // uint32_t setpoint = 5000; // 5 seconds. 
    // digitalWrite(LED_BUILTIN, pt.run(buttonPushed(), setpoint));

}

bool buttonPushed(){
    // When the button is pushed the pin is pulled to the GND and has value FALSE.
    return !digitalRead(BUTTON_PIN);
}