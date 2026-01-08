/* Copyright 2026 Lode Van Dyck
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "TimLib.h"

// ********************************
// TimerMs
// ********************************

// Put the timer in ready state. (only to be used by other timer classes inheriting this class.)
void TimerMs::ready(){
  if(_state == tmrState::running){
    timeRef = elapsedTime();
    _state = tmrState::ready;
  }
}

// constructor resets time
TimerMs::TimerMs () { 
  stop();
};

// calculate the elapsed time and check for overflow. 
uint32_t TimerMs::elapsedTime(){
  
  if(_state == tmrState::running){
    
    //time now
    uint32_t tn = millis();        

    // calculate elapsed time
    // while running timeRef holds the millis() whe the timer started.
    uint32_t et = tn - timeRef;
    
    // handle overflow
    if(et > OVERFLOW_VALUE){
      _state = tmrState::overflow;
      timeRef = OVERFLOW_VALUE;
      return OVERFLOW_VALUE;
    }

    // while running, return the elapsed time.
    return et;
  }

  // All other states return the timeRef value.
  return timeRef;
}

// stop the timer
void TimerMs::stop () { 
  _state = tmrState::idle;
  timeRef = 0;
};

// Pause the timer 
void TimerMs::pause(){
  if(_state == tmrState::running){
    // Store the already elapsed time in the timeRef prop. 
    timeRef = elapsedTime();
    // It might happen that the state has changed to overflow. 
    if(_state == tmrState::running){
      _state = tmrState::paused;
    }
  }
}

// start the timer
// has no effect on an already running timer. 
void TimerMs::start(){
  
  // Start from idle state
  if(_state == tmrState::idle){
    restart();
  }

  // Start from paused state
  else if(_state == tmrState::paused){
    // timeRef holds the already elapsed time. 
    // when running it should hold a reference for millis() to calculate the elapsed time. 
    timeRef = millis() - timeRef;
    _state = tmrState::running;
  }

  // In anny other state, do nothing.
}

// restart (or start) the timer 
void TimerMs::restart(){
  timeRef = millis (); 
  _state = tmrState::running;
}

//return the timer state
tmrState TimerMs::state (){
  return _state;
}

// ********************************
// OnDelayTimer
// ********************************

OnDelayTimer::OnDelayTimer() : TimerMs(){
  
}

OnDelayTimer::OnDelayTimer(uint32_t delayTime_ms) : OnDelayTimer(){
  this->delayTime_ms = delayTime_ms;
}

bool OnDelayTimer::run(bool trigger){
  if(trigger){
    start();  //this only starts the timer when it is not already running. 
    if(elapsedTime() >= delayTime_ms){
      ready();  //Stop the timer from running, go to ready state
    }
  }else{
    stop(); //Stop the timer from running, go to idle state
  }
  return (state() == tmrState::ready);
}

bool OnDelayTimer::run(bool trigger, uint32_t delayTime_ms){
  this->delayTime_ms = delayTime_ms;
  return this->run(trigger);
}

void OnDelayTimer::restart(){
  TimerMs::restart(); 
}

// ********************************
// OffDelayTimer
// ********************************

OffDelayTimer::OffDelayTimer() : TimerMs(){
  // To prevent the timer from starting on program startup the timer is put imediatly in ready state.
  restart();
  ready();
}

OffDelayTimer::OffDelayTimer(uint32_t delayTime_ms) : OffDelayTimer(){
  this->delayTime_ms = delayTime_ms;
}

bool OffDelayTimer::run(bool trigger){
  if(!trigger){
    start();  //this only starts the timer when it is not already running. 
    if(elapsedTime() >= delayTime_ms){
      ready();
    }
  }else{
    stop(); //Stop the timer from running, go to idle state
  }
  return trigger | (state() == tmrState::running);
}

bool OffDelayTimer::run(bool trigger, uint32_t delayTime_ms){
  this->delayTime_ms = delayTime_ms;
  return this->run(trigger);
}

void OffDelayTimer::stop(){
  TimerMs::stop();
}

// ********************************
// PulseTimer
// ********************************

PulseTimer::PulseTimer() : TimerMs(){
  
}

PulseTimer::PulseTimer(uint32_t pulseTime_ms) : PulseTimer(){
  this->pulseTime_ms = pulseTime_ms;
}

bool PulseTimer::run(bool trigger){
  if(trigger){
    start();  //this only starts the timer when it is not already running. 
  }

  if(elapsedTime() >= pulseTime_ms){
    ready();  //Stop the timer from running, go to idle state
  }

  //stop the timer 
  if (!trigger & (state() == tmrState::ready)){
    stop();
  }

  return (state() == tmrState::running);
}

bool PulseTimer::run(bool trigger, uint32_t pulseTime_ms){
  this->pulseTime_ms = pulseTime_ms;
  return this->run(trigger);
}

void PulseTimer::stop(){
  TimerMs::stop();
}

// ********************************
// PauseTimer
// ********************************

PauseTimer::PauseTimer() : TimerMs(){
  
}

PauseTimer::PauseTimer(uint32_t setpointTime_ms) : PauseTimer(){
  this->setpointTime_ms = setpointTime_ms;
}

bool PauseTimer::run(bool trigger){
  if(trigger){
    start();  //this only starts the timer when it is not already running. 
    if(elapsedTime() >= setpointTime_ms){
      ready();  //Stop the timer from running, go to ready state
    }
  }else{
    pause(); //Pause the timer from running
  }
  return (state() == tmrState::ready);
}

bool PauseTimer::run(bool trigger, uint32_t setpointTime_ms){
  this->setpointTime_ms = setpointTime_ms;
  return this->run(trigger);
}

void PauseTimer::reset(){
  stop();
}

// ********************************
// IntervalTimer
// ********************************

IntervalTimer::IntervalTimer() : TimerMs(){
  
}

IntervalTimer::IntervalTimer(uint32_t intervalTime_ms) : IntervalTimer(){
  this->intervalTime_ms = intervalTime_ms;
}

bool IntervalTimer::run(bool trigger){
  if(trigger){
    start();  //this only starts the timer when it is not already running. 
    if(elapsedTime() >= intervalTime_ms){
      timeRef += intervalTime_ms;
      return true;
    }
  }else{
    stop();
  }

  return false;
}

bool IntervalTimer::run(bool trigger, uint32_t intervalTime_ms){
  this->intervalTime_ms = intervalTime_ms;
  return this->run(trigger);
}

bool IntervalTimer::run(){
  return run(true);
}

// ********************************
// LapTimer
// ********************************

LapTimer::LapTimer() : TimerMs(){
  
}

uint32_t LapTimer::lap(){
  if(state() == tmrState::running){
    //normal operation
    uint32_t et = elapsedTime();
    timeRef += et;
    return et;
  }else{
    //first function call.
    start();
    return 0;
  }
}

// ********************************
// CycleTimer
// ********************************

CycleTimer::CycleTimer(){
  reset();
}

void CycleTimer::reset(){         //reset the timer
    max = 0;
    last = 0;
}

void CycleTimer::cycleTriger(){   //call this at the start of each cycle.

  // Elapsed time
  last = lap();

  //Store values
  if(last > max){ max = last; }

}

uint32_t CycleTimer::maxTime(){
  return max;
}

uint32_t CycleTimer::lastTime(){
  return last;
}

// ********************************
// StopWatch
// ********************************

StopWatch::StopWatch() : TimerMs(){
  
}

void StopWatch::start(){
  TimerMs::start();
}

void StopWatch::restart(){
  TimerMs::restart();
}

void StopWatch::stop(){
  TimerMs::stop();
}

uint32_t StopWatch::watch(){
  if(state() == tmrState::running){
    return elapsedTime();
  }else{
    return 0;
  }
}
