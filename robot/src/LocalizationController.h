#pragma one
#include <Arduino.h>
#include "Position.h"
#include "SensorsController.h"
//#include "EngineController.h"

class EngineController;

/**
* Controller responsible for odometry and finding current position.
*/
class LocalizationController
{
  Position _CurrentPosition; ///current position

  double CurrentAngleZ;
  double CurrentAngleX;

  SensorsController* _SensorsController;
  EngineController* _EngineController;

  double _WheelDiameterInMeters;
  int _NumberOfSlots; ///number of slots in encoder disk
  double _AnglePerSlot; ///by what angle will the encoder disk rotate between two slots
  double _WheelRadiusInMeters;

  Alarm _CountingAlarm;

  bool _isRobotLifted;
  bool _isRobotSlipping;
  bool _isRobotStuck;
  
  bool _doesIMUWorks; 

  double _liftedTimeCounter = 0.0;
  double _slipTimeCounter = 0.0;
  double _stuckTimeCounter = 0.0;

  double _previousSpeed = 0.0;

public:
  /**
  * Initializing
  */
  LocalizationController();

  /**
  * It sets wheel diameter and number of slots
  */
  void setup(double wheelDiameterInMeters, int numberOfSlots);

  /**
  * It sets up pointer to the SensorsController
  * @param *controller - pointer to the SensorsController
  */
  void setSensorsController(SensorsController &controller);
  /**
  * It sets up pointer to the EngineController
  * @param *controller - pointer to the EngineController
  */
  void setEngineController(EngineController &controller);

  /**
  * @return true if lifted
  */
  bool isRobotLifted();

  /**
  * @return true if slipping
  */
  bool isRobotSlipping();

  /**
  * It calculates current localization using odometry.
  */
  void update();

  /**
  * @return current robot position
  */
  Position getCurrentPosition();

  /**
  * @return current robot Z angle
  */
  double getCurrentAngleZ();
};