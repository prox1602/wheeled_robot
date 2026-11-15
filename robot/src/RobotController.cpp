#include "RobotController.h"

void RobotController::setup()
{
  _EngineController.setup(LeftEngine1Pin, LeftEngine2Pin, RightEngine1Pin, RightEngine2Pin, MotorsResolution, MotorsFreq);
  _EngineController.attachGoalController(&_GoalController);
  _EngineController.setLocalizationController(&_LocalizationController);

  _SensorsController.setup(LeftEncoderPin, RightEncoderPin, SDAgyroPin, SCLgyroPin);

  _LocalizationController.setup(_WheelDiameterInMeters, _NumberOfSlots);

  _LocalizationController.setSensorsController(_SensorsController);
  _LocalizationController.setEngineController(_EngineController);

  _BluetoothController.setup("ROBOT");

  _GoalController.setBluetoothController(&_BluetoothController);
}

void RobotController::run()
{
  _BluetoothController.update();

  _GoalController.update();

  _SensorsController.update();

  _LocalizationController.update();

  _EngineController.update();
}