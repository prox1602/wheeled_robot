#include "GoalController.h"
#include "BluetoothController.h"

Goal::Goal()  {;}

Goal::Goal(GoalType type, std::shared_ptr<GoalParam> param)
{
  _Type = type;
  _Param = param;
}

GoalType Goal::getType()
{
  return _Type;
}

std::shared_ptr<GoalParam> Goal::getParam()
{
  return _Param;
}

PositionGoalParam::PositionGoalParam(Position position)
{
  _Position = position;
}

Position PositionGoalParam::getPosition()
{
  return _Position;
}

double AngleGoalParam::getAngle()
{
  return _Angle;
}

Goal GoalController::getFrontGoal()
{
  return _Goals.front();
}

void GoalController::popFrontGoal()
{
  _Goals.pop_front();
}

bool GoalController::isEmpty()
{
  return _Goals.empty();
}

void GoalController::update()
{
  if(_BluetoothController->isBufforFull())
  {
    while(_BluetoothController->isBufforFull())
    {
      if(_BluetoothController->getBuffor()->front() == "clear")
      {
        _BluetoothController->getBuffor()->pop();
        _Goals.clear();
      }
      else if(_BluetoothController->getBuffor()->front() == "goto") 
      {
        _BluetoothController->getBuffor()->pop();
          
        loadPosition();
      }
      else if(_BluetoothController->getBuffor()->front() == "rush")
      {
        _BluetoothController->getBuffor()->pop();
        _Goals.clear();
          
        loadPosition();
      }
      else 
      {
        _BluetoothController->getBuffor()->pop();
      }
    }
  }
}

void GoalController::setBluetoothController(BluetoothController* controller)
{
  _BluetoothController = controller;
}

bool GoalController::StringtoDouble(String text, double& result)
{
  const char* input = text.c_str();

  char* end;

  double tmpResult = strtod(input, &end);

  if(end == input)
    return false;

  if(*end != '\0' && *end != '\r' && *end != '\n')
    return false;

  result = tmpResult;
  return true;
}

void GoalController::loadPosition()
{
  double x, y, z = 0.0;

  if(_BluetoothController->isBufforFull())
  {
    String x_text = _BluetoothController->getBuffor()->front();
    
    if(StringtoDouble(x_text, x))
    {
      _BluetoothController->getBuffor()->pop();
    }
    else
    {
      return;
    }
  }
  else return;

  if(_BluetoothController->isBufforFull())
  {
    String y_text = _BluetoothController->getBuffor()->front();
    
    if(StringtoDouble(y_text, y))
    {
      _BluetoothController->getBuffor()->pop();
    }
    else
    {
      return;
    }
  }
  else return;

  if(_BluetoothController->isBufforFull())
  {
    String z_text = _BluetoothController->getBuffor()->front();
    
    if(StringtoDouble(z_text, z))
    {
      _BluetoothController->getBuffor()->pop();
    }
  }
  
  _Goals.push_back(Goal(GoalType::goToPosition, std::make_shared<PositionGoalParam>(Position(x, y, z))));
}

