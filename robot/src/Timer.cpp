#include "Timer.h"

Timer::Timer()
{
  OldTime = double(millis());
}

double Timer::getDelta() const
{
  return (double(millis()) - OldTime) / 1000.0;
}

void Timer::clear()
{
  OldTime = double(millis());
}

Alarm::Alarm(double timeInSec)
  : _Timer()
{
  setLimit(timeInSec);
}

void Alarm::setLimit(double limitInSec)
{
  _LimitInSec = limitInSec;
}

bool Alarm::update()
{
  if(_Timer.getDelta() >= _LimitInSec)
  {
    _Timer.clear();

    return true;
  }
  else return false;
}