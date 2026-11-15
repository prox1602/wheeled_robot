#pragma once
#include <memory>
#include <deque>
#include "Position.h"


class BluetoothController;

enum class GoalType
{
  goToPosition, goToAngle
};

class GoalParam
{
public:
  virtual ~GoalParam() = default;
};

class PositionGoalParam : public GoalParam
{
  Position _Position;

public:
  PositionGoalParam(Position position);

  Position getPosition();
};

class AngleGoalParam : public GoalParam
{
  double _Angle;

public:

  double getAngle();
};

class Goal
{
  GoalType _Type;
  std::shared_ptr<GoalParam> _Param;

public:
  Goal();
  Goal(GoalType type, std::shared_ptr<GoalParam> param);

  GoalType getType();
  std::shared_ptr<GoalParam> getParam();
};

/**
*Controller responsible for setting goals
*/
class GoalController
{
  BluetoothController* _BluetoothController; ///pointer to the BluetoothController

  std::deque <Goal> _Goals; ///a deque containing all registered goals to be achieved

  /**
  * Method converting string to double by reference.
  * @param text - text to be convert
  * @param result - variable by reference in which the value is to be stored
  * @return true if succeed
  */
  bool StringtoDouble(String text, double& result);

  /**
  * It loads and saves new Position to achieve from the BluetoothController.
  */
  void loadPosition();
public:
  /**
  * It check if BluetoothController buffer is full and load new goals to deque
  */
  void update();

  /**
  * It sets up pointer to the BluetoothController
  * @param *controller - pointer to the BluetoothController
  */
  void setBluetoothController(BluetoothController* controller);

  /**
  * @return return front goal from the deque of goals
  */
  Goal getFrontGoal();
  /**
  * delete front goal from the deque of goals
  */
  void popFrontGoal();

  /**
  * It checks if deque of goals is empty
  * @return true if deque is empty
  */
  bool isEmpty();
};

