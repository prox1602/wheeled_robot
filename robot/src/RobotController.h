#pragma once
#include <Arduino.h>
#include "EngineController.h"
#include "LocalizationController.h"
#include "BluetoothController.h"


/**
* Main class responsible for controlling the robot. It does entire logic.
*/
class RobotController
{
  EngineController _EngineController; ///controller of engines and motion
  GoalController _GoalController; ///controller of goal setting
  SensorsController _SensorsController; ///controller of sensors
  LocalizationController _LocalizationController; ///controller of localization and odometry
  BluetoothController _BluetoothController; ///controller of bluetooth communication


  ////SETUP
  int LeftEngine1Pin = 4;   ///pin lewego silinika 1. [eng:] left engine pin num. 1.
  int LeftEngine2Pin = 5;   ///[eng:] left engine pin num. 2.
  int RightEngine1Pin = 7;  ///pin prawego silnika 1. [eng:] right engine pin num. 1.
  int RightEngine2Pin = 6;  ///[eng:] right engine pin num. 2.
  int MotorsFreq = 10000;   ///frequency of PWM pins used to engines control
  int MotorsResolution = 9; ///9 bitow - zakres 0-511; [eng:] PWM pins resolution; 9 is set so it will be range 0-511

  int LeftEncoderPin = 12; 
  int RightEncoderPin = 13;
  int SDAgyroPin = 8;
  int SCLgyroPin = 9;

  double _WheelDiameterInMeters = 0.0655;
  int _NumberOfSlots = 30;

public:
  /**
  * setup method. It is responsible for triggering the configuration of minor controllers 
  */
  void setup();

  /**
  * It runs all program and logic
  */
  void run();
};