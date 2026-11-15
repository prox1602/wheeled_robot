#pragma one
#include <Arduino.h>

/**
* Timer class responsible for calculate time between some points in time. It returns delta time
*/
class Timer
{
  double OldTime;

public:
  /**
  * Calibrating timer
  */
  Timer();

  /**
  * It calculate and returns time elapsed since the last timer reset. IN SECONDS! 
  * @return delta time in seconds
  */
  double getDelta() const;

  /**
  * Reset the timer. It sets new time for OldTime variable
  */
  void clear();
};

/**
* It works simiiarly to Timer but it is used to trigger a specific action after set amount of time has elapsed.
*/
class Alarm
{
  Timer _Timer;

  double _LimitInSec;

public:
  /**
  * Initializing Alarm.
  * @param timeInSec the time interval at which an action should be triggered
  */
  Alarm(double timeInSec);

  /**
  * It sets the limit/period/the time interval at which an action should be triggered
  * @param limitInSec the time interval at which an action should be triggered
  */
  void setLimit(double limitInSec);

  /**
  * Updating the Alarm. It returns true if given time has elapsed and clears the timer.
  */
  bool update();
};