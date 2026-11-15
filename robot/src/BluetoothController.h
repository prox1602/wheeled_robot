#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <queue>
#include <vector>

/**
* Controller responsible for bluetooth communication.
*/
class BluetoothController
{
  BLEServer* _BLServer = nullptr;
  BLEService* _BLService = nullptr;
  BLECharacteristic* TransmitCharacteristic = nullptr;
  BLECharacteristic* ReceiveCharacteristic = nullptr;

  const char* SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
  const char* CHARACTERISTIC_UUID_RX = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
  const char* CHARACTERISTIC_UUID_TX = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

  bool _deviceConnected = false;
  bool _oldDeviceConnected = false;


  std::queue <String> _CommandsBuffor; ///buffer of commands

  /**
  * It convert string text to std::vector of single words.
  * @return std::vector of singe words
  */
  std::vector<String> splitString(String text);

public:
  /**
  * It sets up bluetooth communication
  * @param deviceName name of device
  */
  void setup(const char* deviceName);

  /**
  * Updates itself
  */
  void update();

  /**
  * 
  */
  void setDeviceConnected(bool state);

  /**
  * It get a text message, convert it to single words and push to command buffer
  */
  void receiveCommand(String command);

  /**
  * @return true if full. It means the buffer contains some content.
  */
  bool isBufforFull();

  /**
  * @return pointer to commands buffer
  */
  std::queue <String>* getBuffor();
};

class ServerCallbacks : public BLEServerCallbacks
{
  BluetoothController* _Controller;

public:
  ServerCallbacks(BluetoothController* controller);

  void onConnect(BLEServer* pServer) override;

  void onDisconnect(BLEServer* pServer) override;
};

class ReceiverCallbacks : public BLECharacteristicCallbacks
{
  BluetoothController* _Controller;

public:
  ReceiverCallbacks(BluetoothController* controller);

  void onWrite(BLECharacteristic *pCharacteristic);

};