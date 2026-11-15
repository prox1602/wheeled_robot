#pragma once
#include <Arduino.h>
#include "GoalController.h"
//#include "LocalizationController.h"

class LocalizationController;

/**
* Single engine class. It is use to control the motion of the engine
*/
class Engine
{
	int PIN1;
	int PIN2;

  int maxValue;

  double calculatePower(double value);
public:
  /**
  * setup method of engine. It sets up PWM pins to control the power of engine
  * @param Pin1 - first pin to control the engine
  * @param Pin2 - second pin to control the engine
  * @param MotorsResolution - resolution of PWM
  * @param MotorsFreq - frequency of PWM
  */
  void setup(int Pin1, int Pin2, int MotorsResolution, int MotorsFreq);
  /**
  * update method of engine. It sets new value of engine power;
  * @param Power - value of power to be set
  */
	void update(double Power);	
};

/**
* Engines controller class. It is unit responsible for controlling the engines.
*/
class EngineController
{
  GoalController* _GoalController; ///pointer to the GoalController
  LocalizationController* _LocalizationController; ///pointer to the LocalizationController

  Engine LeftEngine;
  Engine RightEngine;

  double _LeftEnginePower; /// current power set for left engine
  double _RightEnginePower; /// current power set for right engine

  bool _allowReverseMode = false; /// flag - true if driving in reversed is allowed

public:
  /**
  * It sets up pins used to control the engines.
  * @param L1Pin - first pin of left engine 
  * @param L2Pin - second pin of left engine 
  * @param P1Pin - first pin of right engine 
  * @param P2Pin - secodn pin of right engine
  * @param MotorsResolution - resolution of PWM
  * @param MotorsFreq - frequency of PWM
  */
  void setup(int L1Pin, int L2Pin, int P1Pin, int P2Pin, int MotorsResolution, int MotorsFreq);

  /**
  * It sets up pointer to the GoalController
  * @param *goalController - pointer to the GoalController
  */
  void attachGoalController(GoalController* goalController);

  /**
  * It sets up pointer to the LocalizationController
  * @param *controller - pointer to the LocalizationController
  */
  void setLocalizationController(LocalizationController* controller);

  /**
  * It returns value of current power set for left engine
  * @return value of current power set for left engine
  */
  double getLeftEnginePower();
  /**
  * It returns value of current power set for right engine
  * @return value of current power set for right engine
  */
  double getRightEnginePower();

  /**
  * update method. It does all logic of movement and engines control 
  */
  void update();
};