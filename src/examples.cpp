#include <Arduino.h>
#include <TimLib.h>

bool buttonPushed;
bool memoButtonPushed;
OnDelayTimer ondt(5000);          //setpoint 5s.
OffDelayTimer offdt(5000);        //setpoint 5s.
PulseTimer plst(5000);              //setpoint 5s.
IntervalTimer intt(5000);           //setpoint 5s.
LapTimer lpt;
StopWatch stpw;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN2, INPUT_PULLUP);      //button connected from pin 2 to GND

  Serial.begin(9600);
}

void loop() {
  memoButtonPushed = buttonPushed;
  buttonPushed = !digitalRead(PIN2);
  
  // Test On delay timer
  // The led turns on after pressing 5 seconds on the button. 
  digitalWrite(LED_BUILTIN, ondt.run(buttonPushed));

  // Test Off delay timer
  // The led turns on when pussing the button and stays on 5s after the button was released.
  digitalWrite(LED_BUILTIN, offdt.run(buttonPushed));

  // Test Pulse timer
  // Independant of how long the button is pushed, the led will be on for 5 seconds;
  digitalWrite(LED_BUILTIN, plst.run(buttonPushed));

  // Test Interval timer
  // Every 5 seconds a line will be printed. 
  if(intt.run(true)){
    Serial.println("Just another print.");
  }

  // Test lap timer 
  // Print the time between button pushes.
  // Because there is no button debounse handeling there might be more prints then you expect.
  // First push will print value 0 because there is no reference time yet. 
  if(!memoButtonPushed & buttonPushed){
    Serial.println(lpt.lap());
  }

  // Test Stop Watch
  // Print the time the button was pushed. 
  // Because there is no button debounse handeling there might be more prints then you expect.
  if(!memoButtonPushed & buttonPushed){
    // Positive edge, start the stopwatch
    stpw.restart();
  }
  else if(memoButtonPushed & !buttonPushed){
    // negative edge, print elapsed time and stop the stopwatch
    Serial.println(stpw.watch());
    stpw.stop();
  }

}