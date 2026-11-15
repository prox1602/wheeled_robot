#include "LocalizationController.h"
#include "EngineController.h"

LocalizationController::LocalizationController()
  : _CountingAlarm(0.02)
{
  ;
}

void LocalizationController::setup(double wheelDiameterInMeters, int numberOfSlots)
{
  _CurrentPosition.X = 0;
  _CurrentPosition.Y = 0;
  _CurrentPosition.Z = 0;

  CurrentAngleZ = 0.0;

  _WheelDiameterInMeters = wheelDiameterInMeters;
  _NumberOfSlots = numberOfSlots;
  _AnglePerSlot = 360.0 / double(_NumberOfSlots);
  _WheelRadiusInMeters = _WheelDiameterInMeters / 2;

  _isRobotLifted = false;
  _isRobotSlipping = false;
  _isRobotStuck = false;
}

void LocalizationController::setSensorsController(SensorsController &controller)
{
  _SensorsController = &controller;
  _doesIMUWorks = _SensorsController->doesIMUWorks();
}

void LocalizationController::setEngineController(EngineController &controller)
{
  _EngineController = &controller;
}

bool LocalizationController::isRobotLifted()
{
  return _isRobotLifted;
}

bool LocalizationController::isRobotSlipping()
{
  return _isRobotSlipping;
}

void LocalizationController::update()
{
  if(_CountingAlarm.update())
  {
    double deltaTime = 0.02;

    int leftEncoderImpulses = _SensorsController->getLeftEncoderImpulses();
    int rightEncoderImpulses = _SensorsController->getRightEncoderImpulses();

    _SensorsController->clearLeftEncoderImpulses();
    _SensorsController->clearRightEncoderImpulses();

    CurrentAngleZ = _SensorsController->getZAngle();
    CurrentAngleX = _SensorsController->getXAngle();

    ////sprawdzanie czy robot nie zostal podniesiony (wyjscie przyspieszenia pionowego (globalnie) poza zakres)
    double verticalAcceleration = _SensorsController->getVerticalAcceleration();
    if(_doesIMUWorks)
    {
      if(verticalAcceleration < 0.85 || verticalAcceleration > 1.15)
      {
        _liftedTimeCounter += 0.02;

        if(_liftedTimeCounter >= 0.15)
        {
          _isRobotLifted = true;
        }
      }
      else 
      {
        _liftedTimeCounter = 0.0;
      }
    }

    if(_isRobotLifted)
      return;


    ///okreslanie kierunku ruchu poprzez wyznaczenie wersora
    int leftWheelVersor = 0;
    int rightWheelVersor = 0;

    double leftEnginePower = _EngineController->getLeftEnginePower();
    if(leftEnginePower != 0)
      leftWheelVersor = leftEnginePower / abs(leftEnginePower);

    double rightEnginePower = _EngineController->getRightEnginePower();
    if(rightEnginePower != 0)
      rightWheelVersor = rightEnginePower / abs(rightEnginePower);


    ///liczenie pokonanego dystansu
    double leftWheelDistance = leftEncoderImpulses * _AnglePerSlot / 180 * PI * _WheelRadiusInMeters * leftWheelVersor;
    //double leftWheelDistance = leftEncoderImpulses * _AnglePerSlot * _WheelRadiusInMeters * leftWheelVersor;
    //double rightWheelDistance = rightEncoderImpulses * _AnglePerSlot * _WheelRadiusInMeters * rightWheelVersor;
    double rightWheelDistance = rightEncoderImpulses * _AnglePerSlot / 180 * PI * _WheelRadiusInMeters * rightWheelVersor;
  
    double Distance = (leftWheelDistance + rightWheelDistance) / 2;

    ///liczenie predkosci oraz przyspieszenia w celu porownania ich z przyspieszeniem z akcelerometru i okreslenie czy robot nie zakopal sie/sliga sie/ zostal zatrzymany

    double currentSpeed = Distance / deltaTime;
    double encoderAcceleration = (currentSpeed - _previousSpeed) / deltaTime;
    _previousSpeed = currentSpeed;

    double accelIMU_X = _SensorsController->getForwardAcceleration() * 9.81;


    ////poslizg zakopanie
    if(abs(encoderAcceleration - accelIMU_X) > 1.5)
    {
      _slipTimeCounter += deltaTime;

      if(_slipTimeCounter >= 0.2)
      {
        _isRobotSlipping = true;
      }
    }
    else 
    {
      _slipTimeCounter = 0.0;
    }

    //if(_isRobotSlipping)
    //  return;

    ///zablokowanie o przeszkode

    if((_EngineController->getLeftEnginePower() > 0.2 || _EngineController->getRightEnginePower() > 0.2) && abs(currentSpeed) < 0.05)
    {
      _stuckTimeCounter += deltaTime;

      if(_stuckTimeCounter > 0.5)
      {
        _isRobotStuck = true;
      }
    }
    else
    {
      _stuckTimeCounter = 0.0;
    }

    ///aktualizowanie pozycji

    double Z_Distance = Distance * sin(CurrentAngleX * (PI / 180));
    double X_Distance = Distance * cos(CurrentAngleX * (PI / 180)) * cos(CurrentAngleZ * (PI / 180));
    double Y_Distance = Distance * cos(CurrentAngleX * (PI / 180)) * sin(CurrentAngleZ * (PI / 180));

    _CurrentPosition.X += X_Distance;
    _CurrentPosition.Y += Y_Distance;
    _CurrentPosition.Z += Z_Distance;
  }

  Serial.println(String(_CurrentPosition.X) + " " + String(_CurrentPosition.Y) + " " + String(_CurrentPosition.Z) + " " + String(CurrentAngleZ));
}

Position LocalizationController::getCurrentPosition()
{
  return _CurrentPosition;
}

double LocalizationController::getCurrentAngleZ()
{
  return CurrentAngleZ;
}