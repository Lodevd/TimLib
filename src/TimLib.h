#ifndef TIM_LIB_H
#define TIM_LIB_H

#include <Arduino.h>

// All folowing timers use millis() as source for keeping time.
// The time keeping interval is milliseconds.
// The time keeping limit is the Max value of uint32_t.
// Actual overflow hapens at 49 days, 17 hours, 2 minutes, 47 seconds and 295 miliseconds
// To have some safety buffer, overflow is triggerd at 49 days (17 hours before it actualy occurs.)
#define OVERFLOW_VALUE 4233600000   //49 days in milliseconds 

enum class tmrState : uint8_t{
  idle = 0,
  paused = 1,
  running = 2,
  ready = 3,
  overflow = 10
};

// Base timer in milliseconds.
// Counts time from the moment it was started.
// Can be used on its own but also to be inherited for timers with a more specific application.
class TimerMs {
  private:
    tmrState _state;          // State of the timer.

  protected:
    /**
     * @brief The timeRef property stores different values depending on the state of the timer.
     * while paused :   Stores the already elapsed time while running.
     * while overflow : Stores the OVERFLOW_VALUE
     * while running :  Stores the reference millis() value to calculate the elapsed time. 
     * while idle :     Stores 0.
     * while ready :    Stores the elapsed time the moment the timer was ready. 
     */
    uint32_t timeRef;         // Start time while running, or elapsed time while paused. 
    void ready();             // Changes the state to ready. (only to be used by other timer classes inheriting this class.)
    
  public:
    TimerMs();                // Constructor
    uint32_t elapsedTime();   // Get the elapsed time.
    void stop();              // Stop the timer or clear the overflow error. (puts the timer in idle state)
    void pause();             // Pause the timer. 
    void start();             // Start the timer from idle or paused state. (Does nothing when the timer is already running.) 
    void restart();           // To restart a running timer from 0 ms. (Will work from anny state the timer is in.)
    tmrState state();         // Get the timer state.
};

// The timer will be ready (return true) after the delay has passed.
// The delays starts when then trigger input is high. 
// When the trigger input falls to False, the delay time is reset to 0ms. (Will need to start all over.) 
class OnDelayTimer : private TimerMs{
  private:
    uint32_t delayTime_ms;

  public:
    OnDelayTimer();
    OnDelayTimer(uint32_t delayTime_ms);
    bool run(bool trigger);
    bool run(bool trugger, uint32_t delayTime_ms);
    void restart();       // Restart the delay timer. 
};

// The timer will be ready (return true) while running (trigger = true) and during the delay after. 
class OffDelayTimer : private TimerMs{
  private:
    uint32_t delayTime_ms;

  public:
    OffDelayTimer();
    OffDelayTimer(uint32_t delayTime_ms);
    bool run(bool trigger);
    bool run(bool trigger, uint32_t delayTime_ms);
    void stop();        // Early stop (return false) of the timer. 
};

// The timer will give a fixed length pulse starting on the positive edge of the trigger signal. 
// The input signal can be longer or shorter then the pulse time. 
// A second positive edge while the pulse is active will not increase the pulse length.  
class PulseTimer : private TimerMs{
  private:
    uint32_t pulseTime_ms;

  public:
    PulseTimer();
    PulseTimer(uint32_t pulseTime_ms);
    bool run(bool trigger);
    bool run(bool trigger, uint32_t pulseTime_ms);
    void stop();    // Early stopping the pulse signal. 
};

// The timer runs while the run trigger is high. 
// When the trigger is false the timer pauses and continues when the trigger is high again. 
// When the timer reaches the setpoint it stops timing and returns true. 
// The timer keeps returning true until it receives a reset command. 
class PauseTimer : private TimerMs{
  private:
    uint32_t setpointTime_ms;

  public:
    PauseTimer();
    PauseTimer(uint32_t setpointTime_ms);
    bool run(bool trigger);
    bool run(bool trigger, uint32_t setpointTime_ms);
    void reset();
};

/**
 * @brief This timer gives a 1 cycle pulse output (return true) and imediatly restarts. 
 * This way you can execute code at a fixed time interval.
 */
class IntervalTimer : private TimerMs{
  private:
    uint32_t intervalTime_ms;

  public:
    IntervalTimer();
    IntervalTimer(uint32_t intervalTime_ms);
    bool run(bool trigger);
    bool run(bool trigger, uint32_t intervalTime_ms);
    bool run();
};

/**
 * @brief This returns te passed time since the last call. 
 * For example to analyze code execution times. 
 */
class LapTimer : private TimerMs{
  private:

  public:
    LapTimer();
    uint32_t lap();   //returns the passed time since the previous call. 
};

/**
 * @brief An extension of LapTimer. 
 * It keeps track of the last and maximum duration of a repeating cycle. For example loop().
 * 
 */
class CycleTimer : private LapTimer{
  private:

    uint32_t max = 0;
    uint32_t last = 0;

  public:
    CycleTimer();
    void cycleTriger();   //call this at the start of each cycle.
    void reset();         //reset the timer
    uint32_t maxTime();   //returns the maximum measured cycle time
    uint32_t lastTime();  //returns the last measured cycle time
};

/**
 * @brief Start the timer at a sertain point. 
 * Then read out the passed time since start. 
 * You can read out the passed time as manny times as needed. 
 * The timer can be restarted and stoped. 
 */
class StopWatch : private TimerMs{
  private:

  public:
    StopWatch();
    void start();       // Start the timer from idle state. (Does nothing when the timer is already running.)
    void restart();     // Restart the timer from 0. (Can be used from anny state.) 
    void stop();        // Stop the timer.
    uint32_t watch();   // Returns the passed time since the previous call. 
};

#endif
