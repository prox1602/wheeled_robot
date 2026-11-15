#include "BluetoothController.h"

ServerCallbacks::ServerCallbacks(BluetoothController* controller)
{
  _Controller = controller;
}

void ServerCallbacks::onConnect(BLEServer* pServer)
{
  _Controller->setDeviceConnected(true);
}

void ServerCallbacks::onDisconnect(BLEServer* pServer)
{
  _Controller->setDeviceConnected(false);
}

ReceiverCallbacks::ReceiverCallbacks(BluetoothController* controller)
{
  _Controller = controller;
}

void ReceiverCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  String rxValue = pCharacteristic->getValue().c_str();
  if(rxValue.length() > 0)
  {
    rxValue.trim();
    _Controller->receiveCommand(rxValue);
  }
}


void BluetoothController::setup(const char* deviceName)
{
  BLEDevice::init(deviceName);

  _BLServer = BLEDevice::createServer();\

  _BLServer->setCallbacks(new ServerCallbacks(this));

  _BLService = _BLServer->createService(SERVICE_UUID);

  TransmitCharacteristic = _BLService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  TransmitCharacteristic->addDescriptor(new BLE2902());

  ReceiveCharacteristic = _BLService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  ReceiveCharacteristic->setCallbacks(new ReceiverCallbacks(this));


  _BLService->start();
  _BLServer->getAdvertising()->start();
}

void BluetoothController::update()
{
  if(!_deviceConnected && _oldDeviceConnected)
  {
    delay(50);
    _BLServer->startAdvertising();
    Serial.println("Rozlaczono z urzadzeniem. Wznawiam szukanie...");
    _oldDeviceConnected = _deviceConnected;
  }

  if(_deviceConnected && !_oldDeviceConnected)
  {
    _oldDeviceConnected = _deviceConnected;
    Serial.println("Urzadzenie podlaczone!");
  }
}

void BluetoothController::setDeviceConnected(bool state)
{
  _deviceConnected = state;
}

void BluetoothController::receiveCommand(String command)
{
  command.toLowerCase();

  std::vector<String> commands = splitString(command);

  for(int i = 0; i < commands.size(); i++)
  {
    _CommandsBuffor.push(commands[i]);
  }
}

std::vector<String> BluetoothController::splitString(String text)
{
  std::vector<String> result;

  int beginIndex = 0;

  int spaceIndex = text.indexOf(' ');

  while(spaceIndex >= 0)
  {
    result.push_back(String(text.substring(beginIndex, spaceIndex)));

    beginIndex = spaceIndex + 1;

    spaceIndex = text.indexOf(' ', beginIndex);
  }

  if(beginIndex < text.length())
  {
    result.push_back(String(text.substring(beginIndex)));
  }

  return result;
}

bool BluetoothController::isBufforFull()
{
  if(_CommandsBuffor.size() > 0)
    return true;
  else return false;
}

std::queue <String>* BluetoothController::getBuffor()
{
  return &_CommandsBuffor;
}