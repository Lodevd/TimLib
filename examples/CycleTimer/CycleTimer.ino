/*
 Cycle timer

 It keeps track of the last and maximum duration of a repeating cycle. For example loop().

 Created  7 Jan 2026
 by Lode Van Dyck
*/

#include <TimLib.h>

// Create an instance of an interval timer, with 5000ms interval. 
IntervalTimer printTimer(5000);     // Setpoint set to 5000ms.
CycleTimer ct;                      // The timer to measure the cycle time. 

void setup() {
  // Serial to create some output. 
  Serial.begin(9600);
}

void loop() {
  
  // Every 5 seconds we print the output
  if(printTimer.run()){
    Serial.print("Last, longest cycle times: ");
    Serial.print(ct.lastTime());
    Serial.print("ms, ");
    Serial.print(ct.maxTime());
    Serial.println("ms");
  }

  // Random delay between 0 and 1000ms. To affect the cycle time.  
  delay(random(1000));

  // Time each iteration of loop. 
  ct.cycleTriger();
}
