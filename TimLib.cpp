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
    uint32_t et = (tn >= timeRef) ? (tn - timeRef) : ((UINT32_MAX - timeRef) + tn);
    
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
// OnDelayTimerMs
// ********************************

OnDelayTimerMs::OnDelayTimerMs() : TimerMs(){
  
}

OnDelayTimerMs::OnDelayTimerMs(uint32_t delayTime_ms) : OnDelayTimerMs(){
  this->delayTime_ms = delayTime_ms;
}

bool OnDelayTimerMs::run(bool trigger){
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

bool OnDelayTimerMs::run(bool trigger, uint32_t delayTime_ms){
  this->delayTime_ms = delayTime_ms;
  return this->run(trigger);
}

void OnDelayTimerMs::restart(){
  TimerMs::restart(); 
}

// ********************************
// OffDelayTimerMs
// ********************************

OffDelayTimerMs::OffDelayTimerMs() : TimerMs(){
  // To prevent the timer from starting on program startup the timer is put imediatly in ready state.
  restart();
  ready();
}

OffDelayTimerMs::OffDelayTimerMs(uint32_t delayTime_ms) : OffDelayTimerMs(){
  this->delayTime_ms = delayTime_ms;
}

bool OffDelayTimerMs::run(bool trigger){
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

bool OffDelayTimerMs::run(bool trigger, uint32_t delayTime_ms){
  this->delayTime_ms = delayTime_ms;
  return this->run(trigger);
}

void OffDelayTimerMs::stop(){
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

void PauseTimer::setpoint(uint32_t setpointTime_ms){
  this->setpointTime_ms = setpointTime_ms;
}

void PauseTimer::start(){
  TimerMs::start();  //this only starts the timer when it is not already running. 
}

void PauseTimer::start(uint32_t setpointTime_ms){
  this->setpointTime_ms = setpointTime_ms;
  this->start();
}

void PauseTimer::pause(){
  TimerMs::pause();
}

void PauseTimer::stop(){
  TimerMs::stop();
}

bool PauseTimer::running(){
  return (state() == tmrState::running);
}

PauseTimer::operator bool(){
  return running();
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
    max = UINT32_MAX;
    last = 0;
}

void CycleTimer::cycleTriger(){   //call this at the start of each cycle.
    
  //time now
  uint32_t tn = millis();

  // calculate elapsed time
  // while running timeRef holds the millis() whe the timer started.
  uint32_t et = (tn >= timeRef) ? (tn - timeRef) : ((UINT32_MAX - timeRef) + tn);

  //Store time reference
  timeRef = tn;

  //First time, ony store timer reference value, do not store values.
  if(max == UINT32_MAX){
    max = 0;
    return;
  }

  //Store values
  last = et;
  if(et > max) max = et;

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
