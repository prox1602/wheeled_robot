#include "EngineController.h"
#include "LocalizationController.h"

void Engine::setup(int Pin1, int Pin2, int MotorsResolution, int MotorsFreq)
{
  ledcAttach(Pin1, MotorsFreq, MotorsResolution);
  ledcAttach(Pin2, MotorsFreq, MotorsResolution);

  PIN1 = Pin1;
  PIN2 = Pin2;

  ledcWrite(PIN1, 0);
  ledcWrite(PIN2, 0);

  maxValue = pow(2, MotorsResolution) - 1;
}

double Engine::calculatePower(double value)
{
  double constrainedValue = constrain(value, -1.0, 1.0);

  if(constrainedValue > 0)
  {
    return 0.68 * maxValue + (constrainedValue * (maxValue - 0.68 * maxValue));
  }
  else if(constrainedValue < 0)
  {
    return 0.68 * maxValue + ((-constrainedValue) * (maxValue - 0.68 * maxValue));
  }
  else if(constrainedValue == 0.0)
    return 0;

  return 0;
}

void Engine::update(double Power)
{

  if(Power > 0)
  {
    ledcWrite(PIN1, calculatePower(Power));
    ledcWrite(PIN2, 0);
  }
  else if(Power < 0)
  {
    ledcWrite(PIN1, 0);
    ledcWrite(PIN2, calculatePower(Power));
  }
  else 
  {
    ledcWrite(PIN1, 0);
    ledcWrite(PIN2, 0);
  }
}

void EngineController::setup(int L1Pin, int L2Pin, int P1Pin, int P2Pin, int MotorsResolution, int MotorsFreq)
{
  _GoalController = nullptr;
  LeftEngine.setup(L1Pin, L2Pin, MotorsResolution, MotorsFreq);
  RightEngine.setup(P1Pin, P2Pin, MotorsResolution, MotorsFreq);

  _LeftEnginePower = 0.0;
  _RightEnginePower = 0.0;
}

void EngineController::attachGoalController(GoalController* goalController)
{
  _GoalController = goalController;
}

void EngineController::setLocalizationController(LocalizationController* controller)
{
  _LocalizationController = controller;
}

double EngineController::getLeftEnginePower()
{
  return _LeftEnginePower;
}

double EngineController::getRightEnginePower()
{
  return _RightEnginePower;
}

void EngineController::update()
{

  //if(!_LocalizationController->isRobotLifted() && !_LocalizationController->isRobotSlipping())
  if(!_LocalizationController->isRobotLifted())
  {

    ////obliczenia na kwadracie odleglosci

    if(!_GoalController->isEmpty())
    {
      Goal goal = _GoalController->getFrontGoal();

      int speedMultiplier = 1;

      if(goal.getType() == GoalType::goToPosition)
      {
        std::shared_ptr<PositionGoalParam> param = std::static_pointer_cast<PositionGoalParam>(goal.getParam());

        double Distance = pow(param->getPosition().X - _LocalizationController->getCurrentPosition().X, 2) + pow(param->getPosition().Y - _LocalizationController->getCurrentPosition().Y, 2);

        if(Distance < 0.025*0.025)
        {
          _GoalController->popFrontGoal();
          _LeftEnginePower = 0.0;
          _RightEnginePower = 0.0;
          LeftEngine.update(_LeftEnginePower);
          RightEngine.update(_RightEnginePower);
          return;
        }
        else if(Distance < 0.1 * 0.1)   {   speedMultiplier = 1;    }
        else if(Distance < 0.5*0.5)   {   speedMultiplier = 2;    }
        else if(Distance < 1*1)   {   speedMultiplier = 3;    }
        else if(Distance < 3*3)   {   speedMultiplier = 4;    }
        else if(Distance < 5*5)   {   speedMultiplier = 5;    }

        double GoalAngle = atan2(param->getPosition().Y - _LocalizationController->getCurrentPosition().Y, param->getPosition().X - _LocalizationController->getCurrentPosition().X);

        double AngleDifference = GoalAngle * 180 / PI - _LocalizationController->getCurrentAngleZ();

        if(AngleDifference > 180)
          AngleDifference -= 360;
        else if(AngleDifference < -180)
          AngleDifference += 360;


        double baseSpeed = 0.2 * speedMultiplier;

        if(_allowReverseMode)
        {
          if(abs(AngleDifference) > 90)
          {
            baseSpeed = -0.2 * speedMultiplier;

            if(AngleDifference > 0)
              AngleDifference -= 180.0;
            else
              AngleDifference += 180.0;

          }
        }

        double turnSpeed = 0.0;
        double Kp = 0.03;

        turnSpeed = AngleDifference * 0.015;

        _LeftEnginePower = baseSpeed + turnSpeed;
        _RightEnginePower = baseSpeed - turnSpeed;

      }
    }
    else
    {
      _LeftEnginePower = 0;
      _RightEnginePower = 0;
    }

  }
  else 
  {
    _LeftEnginePower = 0;
    _RightEnginePower = 0;
  }
  LeftEngine.update(_LeftEnginePower);
  RightEngine.update(_RightEnginePower);
}
