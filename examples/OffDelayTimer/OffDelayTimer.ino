/*
 Off Delay timer

 The timer will be ready (return true) while running (trigger = true) and during the delay after. 

 This example is for the aruino uno or other bords with a build in led. 'LED_BUILTIN'
 Connect a button between the 'BUTTON_PIN' and GND. 

 Created  4 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

#define BUTTON_PIN PIN2    // Change this to the pin you will be using. 

bool buttonPushed();

// Create an instance of an off delay timer, with 2000ms delay. 
OffDelayTimer odt(2000);     // Off delay time setpoint set to 2000ms.
// OffDelayTimer odt;        // No initial setpoint. 

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // input pin to trigger the timer. 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  
    // The led will turn on imediatly when the button is pussed.
    // It turns off two seconds after the button is released. 
    digitalWrite(LED_BUILTIN, odt.run(buttonPushed()));

    // You can also specify the delay time while calling run. 
    // uint32_t setpoint = 5000; // 5 seconds. 
    // digitalWrite(LED_BUILTIN, odt.run(buttonPushed(), setpoint));

}

bool buttonPushed(){
    // When the button is pushed the pin is pulled to the GND and has value FALSE.
    return !digitalRead(BUTTON_PIN);
}