/*
 Interval timer

 This timer gives a 1 cycle pulse output (return true) and imediatly restarts. 
 This way you can execute code at a fixed time interval.

 Created  4 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

// Create an instance of an interval timer, with 5000ms interval. 
IntervalTimer it(5000);     // Setpoint set to 5000ms.
// IntervalTimer it;        // No initial setpoint. 

void setup() {
  // Serial to create some output. 
  Serial.begin(9600);
}

void loop() {
  
  // Every 5 seconds a line will be printed. 
  if(it.run()){
    Serial.println("This line is printed every 5 seconds.");
  }

  // You can control the running of the timer.
  // bool trigger; 
  // if(it.run(trigger))

  // And you can change the interval while running 
  // uint32_t setpoint = 5000; 
  // if(it.run(trigger, setpoint))

}
