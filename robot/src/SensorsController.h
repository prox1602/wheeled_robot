#pragma one
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <ESP32Encoder.h>
#include "Timer.h"

/**
* That class is responsible for controlling the sensors, reading data and sharing it to others controllers.
*/
class SensorsController
{

  int PIN_LeftEncoder;
  int PIN_RightEncoder;

  int PIN_GyroSDA;
  int PIN_GyroSCL;

  //static volatile long LeftEncoderImpulses;
  //static volatile long RightEncoderImpulses;

  //static void IRAM_ATTR countLeftEncoderImpulses();
  //static void IRAM_ATTR countRightEncoderImpulses();

  ESP32Encoder _LeftEncoder;
  ESP32Encoder _RightEncoder;


  BMI270 IMU;
  bool _doesIMUWorks;
  double gyroScale = 3.994;  ///gyro correction factor

  Timer _Timer;

  double Z_Angle = 0.0;
  double gyroDriftZ = 0.0;

  double X_Angle = 0.0;
  double gyroDriftX = 0.0;

  double Y_Angle = 0.0;
  double gyroDriftY = 0.0;

  double _Accel_X = 0.0;
  double _Accel_Y = 0.0;
  double _Accel_Z = 0.0;

  double _VerticalAcceleration = 0.0;

public:
  /**
  * It initializes all sensors such as encoders and initializes . During the first few seconds, it performs a calibration of the gyroscope.
  */
  void setup(int leftEnc, int rightEnc, int SDA, int SCL);

  /**
  * It calibrates gyro's Z axis and calculates the Z drift
  */
  void calculateGyroZDrift();
  /**
  * It calibrates gyro's X axis and calculates the X drift
  */
  void calculateGyroXDrift();
  /**
  * It calibrates gyro's Y axis and calculates the Y drift
  */
  void calculateGyroYDrift();

  /**
  * It performs all the measurements.
  */
  void update();

  /**
  * @return numer of left encoder's impulses
  */
  int getLeftEncoderImpulses();

  /**
  * It clears left encoder impulses
  */
  void clearLeftEncoderImpulses();

  /**
  * @return numer of right encoder's impulses
  */
  int getRightEncoderImpulses();
  /**
  * It clears right encoder impulses
  */
  void clearRightEncoderImpulses();

  /**
  * @return Z angle
  */
  double getZAngle();
  /**
  * @return X Angle
  */
  double getXAngle();

  double getVerticalAcceleration(); ///in g's
  double getForwardAcceleration(); ///in g's;

  bool doesIMUWorks();
};