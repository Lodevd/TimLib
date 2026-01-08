# TimLib
Library with different timers for the arduino framework.

Timers in this library:
  - On delay timer 
  - Off delay timer
  - Pulse timer
  - Pauseable timer
  - Interval timer
  - Lap timer
  - Cycle timer
  - Stop watch timer

## Timers ##

Below are short snipets of the basics u need to know to use the timers. 
There is an examples folder with a more complete example for each timer. 

### On delay timer ###

The timer will be ready (return true) after the delay has passed.
The delays starts when then trigger input is high. 
When the trigger input falls to False, the delay time is reset to 0ms. (Will need to start all over.) 

```C++
bool trigger;
bool result;

// fixed setpoint 
OnDelayTimer myTimer(2000);   // On delay time setpoint set to 2000ms.
result = myTimer.run(trigger);

// Dynamic setpoint 
OnDelayTimer myTimer;         // No initial setpoint.
uint32_t setpoint = 5000; // 5 seconds. 
result = myTimer.run(trigger, setpoint);
```

### Off delay timer ###

The timer will be ready (return true) while running (trigger = true) and during the delay after. 

```C++
bool trigger;
bool result;

// fixed setpoint 
OffDelayTimer myTimer(2000);   // Off delay time setpoint set to 2000ms.
result = myTimer.run(trigger);

// Dynamic setpoint 
OffDelayTimer myTimer;         // No initial setpoint.
uint32_t setpoint = 5000;  // 5 seconds. 
result = myTimer.run(trigger, setpoint);
```

### Pulse timer ###

The timer will give a fixed length pulse starting on the positive edge of the trigger signal. 
The input signal can be longer or shorter then the pulse time. 
A second positive edge while the pulse is active will not increase the pulse length.  

```C++
bool trigger;
bool result;

// fixed setpoint 
PulseTimer myTimer(2000);   // Pulse time setpoint set to 2000ms.
result = myTimer.run(trigger);

// Dynamic setpoint 
PulseTimer myTimer;         // No initial setpoint.
uint32_t setpoint = 5000;  // 5 seconds. 
result = myTimer.run(trigger, setpoint);
```

### Pauseable timer ###

The timer runs while the run trigger is high. 
When the trigger is false the timer pauses and continues when the trigger is high again. 
When the timer reaches the setpoint it stops timing and returns true. 
The timer keeps returning true until it receives a reset command. 

```C++
bool trigger;
bool result;

// fixed setpoint 
PauseTimer myTimer(2000);   // Pulse time setpoint set to 2000ms.
result = myTimer.run(trigger);

// Dynamic setpoint 
PauseTimer myTimer;         // No initial setpoint.
uint32_t setpoint = 5000;  // 5 seconds. 
result = myTimer.run(trigger, setpoint);

// This timer nees to be reset when its ready. 
myTimer.reset();
```

### Interval timer ###

This timer gives a 1 cycle pulse output (return true) and imediatly restarts. 
This way you can execute code at a fixed time interval.

```C++
bool trigger;

// fixed setpoint 
IntervalTimer myTimer(2000);   // Pulse time setpoint set to 2000ms.
if(myTimer.run(trigger)){
  // code to execute at an interval here. 
}

// If you do not need a trigger but just run constantly. 
if(myTimer.run()){
  // code to execute at an interval here. 
}

// Dynamic setpoint 
IntervalTimer myTimer;         // No initial setpoint.
uint32_t setpoint = 5000;  // 5 seconds. 
if(myTimer.run(trigger, setpoint)){
  // code to execute at an interval here. 
}

```

### Lap timer ###

This returns te passed time since the previous call. 
For example to analyze code execution times. 

```C++

LapTimer myTimer;

// Now in your code just call the lap() function to know how much time has passed since the previous lap. 
uint32_t lapTimer = myTimer.lap();

```

### Cycle timer ###

An extension of LapTimer. 
It keeps track of the last and maximum duration of a repeating cycle. For example loop().

```C++

CycleTimer myTimer;

// Somewhere in the cyle call this.
myTimer.cycleTriger();

// Whenever you want to analyze the cycle time you can use 
uint32_t time;
time = myTimer.lastTime();
// And/or
time = myTimer.maxTime();

// To reset the max value. 
myTimer.reset();

```

### Stop watch timer ###

Start the timer at a sertain point. 
Then read out the passed time since start. 
You can read out the passed time as manny times as needed. 
The timer can be restarted and stoped. 

```C++

StopWatch myTimer;

// To start
myTimer.start();    // Only works when stopped
// Or 
myTimer.restart();    // Always works

// Whatch what time has passed since start
uint32_t time;
time = myTimer.watch();

// And to stop 
myTimer.stop()

```

## License ##

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA