#include "SensorsController.h"

/*
volatile long SensorsController::LeftEncoderImpulses = 0;
volatile long SensorsController::RightEncoderImpulses = 0;
*/

void SensorsController::setup(int leftEnc, int rightEnc, int SDA, int SCL)
{
  Serial.begin(115200);

  PIN_LeftEncoder = leftEnc;
  PIN_RightEncoder = rightEnc;
  PIN_GyroSDA = SDA;
  PIN_GyroSCL = SCL;

  //attachInterrupt(PIN_LeftEncoder, countLeftEncoderImpulses, CHANGE);
  //attachInterrupt(PIN_RightEncoder, countRightEncoderImpulses, CHANGE);

  int DUMMY_PIN_LEFT = 15;
  int DUMMY_PIN_RIGHT = 16;

  _LeftEncoder.attachSingleEdge(PIN_LeftEncoder, DUMMY_PIN_LEFT);
  _RightEncoder.attachSingleEdge(PIN_RightEncoder, DUMMY_PIN_RIGHT); 

  _LeftEncoder.clearCount();
  _RightEncoder.clearCount();

  Wire.begin(PIN_GyroSDA, PIN_GyroSCL);

  delay(100);

  if(IMU.beginI2C(BMI2_I2C_PRIM_ADDR, Wire) == BMI2_OK)
  {
    Serial.println("BMI270 uruchomiony pomyslnie!");
    _doesIMUWorks = true;

    calculateGyroZDrift();
    calculateGyroXDrift();
    calculateGyroYDrift();
  }
  else if(IMU.beginI2C(BMI2_I2C_SEC_ADDR, Wire) == BMI2_OK)
  {
    Serial.println("BMI270 uruchomiony pomyslnie!");
    _doesIMUWorks = true;

    calculateGyroZDrift();
    calculateGyroXDrift();
    calculateGyroYDrift();

  }
  else {
    Serial.println("Nie wykryto czujnika BMI270!");
    _doesIMUWorks = false;
    ESP.restart();
  }

}

void SensorsController::calculateGyroZDrift()
{
  double sum = 0.0;
  for(int i = 0; i < 100; i++)
  {
    IMU.getSensorData();
    sum += IMU.data.gyroZ;
    delay(10);
  }

  sum /= 100;

  gyroDriftZ = sum;
}

void SensorsController::calculateGyroXDrift()
{
  double sum = 0.0;
  for(int i = 0; i < 100; i++)
  {
    IMU.getSensorData();
    sum += IMU.data.gyroX;
    delay(10);
  }

  sum /= 100;

  gyroDriftX = sum;
}

void SensorsController::calculateGyroYDrift()
{
  double sum = 0.0;
  for(int i = 0; i < 100; i++)
  {
    IMU.getSensorData();
    sum += IMU.data.gyroY;
    delay(10);
  }

  sum /= 100;

  gyroDriftY = sum;
}

void SensorsController::update()
{
  double deltaTime = _Timer.getDelta();

  if (deltaTime <= 0.0001) 
  {
    return; 
  }

  if(IMU.getSensorData() == BMI2_OK)
  {
    Z_Angle += (IMU.data.gyroZ - gyroDriftZ) * gyroScale * deltaTime;
    X_Angle += (IMU.data.gyroX - gyroDriftX) * gyroScale * deltaTime;
    Y_Angle += (IMU.data.gyroY - gyroDriftY) * gyroScale * deltaTime;


    //
    while(Z_Angle > 180)  { Z_Angle -= 360.0; }
    
    while(Z_Angle < -180) { Z_Angle += 360.0; }

    //
    while(X_Angle > 180)  { X_Angle -= 360.0; }

    while(X_Angle < -180) { X_Angle += 360.0; }

    //
    while(Y_Angle > 180)  { Y_Angle -= 360.0; }
    
    while(Y_Angle < -180) { Y_Angle += 360.0; }


    //Serial.println(String(Z_Angle) + " " + String(X_Angle) + " " + String(Y_Angle));

    float rawAccelX = IMU.data.accelX;
    float rawAccelY = IMU.data.accelY;
    float rawAccelZ = -IMU.data.accelZ; ///czujnik jest do gory nogami

    _Accel_X = rawAccelX * 0.1 + _Accel_X * 0.9;
    _Accel_Y = rawAccelY * 0.1 + _Accel_Y * 0.9;
    _Accel_Z = rawAccelZ * 0.1 + _Accel_Z * 0.9;


    double Z_from_X = _Accel_X * sin(Y_Angle * (PI / 180));
    double Z_from_Y = _Accel_Y * sin(X_Angle * (PI / 180)) * cos(Y_Angle * (PI / 180));
    double Z_from_Z = _Accel_Z * cos(X_Angle * (PI / 180)) * cos(Y_Angle * (PI / 180));

    _VerticalAcceleration = Z_from_X + Z_from_Y + Z_from_Z;

    //Serial.println(String(_VerticalAcceleration));
  }

  _Timer.clear();
}

/*
void IRAM_ATTR SensorsController::countLeftEncoderImpulses()
{
  LeftEncoderImpulses++;
}

void IRAM_ATTR SensorsController::countRightEncoderImpulses()
{
  RightEncoderImpulses++;
}
*/

void update();

int SensorsController::getLeftEncoderImpulses()
{
  return abs((int)_LeftEncoder.getCount());
}

void SensorsController::clearLeftEncoderImpulses()
{
  _LeftEncoder.clearCount();
}

int SensorsController::getRightEncoderImpulses()
{
  return abs((int)_RightEncoder.getCount());
}

void SensorsController::clearRightEncoderImpulses()
{
  _RightEncoder.clearCount();
}

double SensorsController::getZAngle()
{
  return Z_Angle;
}

double SensorsController::getXAngle()
{
  return X_Angle;
}

double SensorsController::getVerticalAcceleration()
{
  return _VerticalAcceleration;
}

double SensorsController::getForwardAcceleration()
{
  return _Accel_X - sin(Y_Angle * (PI / 180.0));
}

bool SensorsController::doesIMUWorks()
{
  return _doesIMUWorks;
}