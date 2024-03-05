//Import needed header files
#include "BLEDevice.h"

//Define characteristics of BLE Server
#define bleServerName "LED Service"
static BLEUUID ledServiceUUID("d934e3ea-4a99-4229-807c-2eeb021e3b7a");
static BLEUUID redCharacteristicUUID("4f2ec411-cdc6-4e42-8705-6221448f3ba3");
static BLEUUID greenCharacteristicUUID("c34faf8c-a288-4dec-ba29-8bd32a9036c5");
static BLEUUID blueCharacteristicUUID("3dc94628-3be5-4676-867d-d6a4bd6b5d91");

//Connectivity variables
static bool doConnect = false;
static bool connected = false;

//Server details
static BLEAddress *pServerAddress;
static BLERemoteCharacteristic* redCharacteristic;
static BLERemoteCharacteristic* greenCharacteristic;
static BLERemoteCharacteristic* blueCharacteristic;

//Activate notifications
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

//Variables to store LED status
char* redChar;
char* greenChar;
char* blueChar;

//Checkers for whether new readings are available
bool newRed = false;
bool newBlue = false;
bool newGreen = false;

//Define LED pins
const int redPin = 21;
const int bluePin = 23;
const int greenPin = 22;

//Connect to the BLE Server that has matching name, service, and characteristics
bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient();
 
  //Connect to the BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");
 
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(bleServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(bleServiceUUID.toString().c_str());
    return (false);
  }
 
  //Obtain a reference to the characteristics
  redCharacteristic = pRemoteService->getCharacteristic(redCharUUID);
  blueCharacteristic = pRemoteService->getCharacteristic(blueCharUUID);
  greenCharacteristic = pRemoteService->getCharacteristic(greenCharUUID);

  if (redCharacteristic == nullptr || blueCharacteristic == nullptr || greenCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
 
  //Assign callback functions for the Characteristics
  redCharacteristic->registerForNotify(redNotifyCallback);
  blueCharacteristic->registerForNotify(blueNotifyCallback);
  greenCharacteristic->registerForNotify(greenNotifyCallback);
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    //If advertised name matches
    if (advertisedDevice.getName() == bleServerName) {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      Serial.println("Device found. Connecting!");
    }
  }
};

//When server sends notify of red characteristic
static void redNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  redChar = (char*)pData;
  newRed = true;
}

//When server sends notify of blue characteristic
static void blueNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  blueChar = (char*)pData;
  newBlue = true;
}

//When server sends notify of green characteristic
static void greenNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  //store temperature value
  greenChar = (char*)pData;
  newGreen = true;
}

void setup() {
  //Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Starting BLE client...");

  //Initialize BLE device
  BLEDevice::init("");

  //Start scanning
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  //Initialize pin modes
  pinMode(greenPin,OUTPUT);

  //Initialize pin values
  digitalWrite(greenPin,LOW);
}

void loop() {
  if(doConnect == true){
    if(connectToServer(*pServerAddress)){
      Serial.println("Connected to server");
      connected = true;
    } else {
      Serial.println("Failed to connect to server, restart device");
    }
    doConnect = false;
  }

  //If newReadings are available
  if (newRed || newGreen || newBlue){
    if((int)redChar == 1){
      digitalWrite(redPin,HIGH);
      digitalWrite(bluePin,LOW);
      digitalWrite(greenPin,LOW);
    }

    if((int)blueChar == 1){
      digitalWrite(bluePin,HIGH);
      digitalWrite(greenPin,LOW);
      digitalWrite(redPin,LOW);
    }

    if((int)greenChar == 1){
      digitalWrite(greenPin,HIGH);
      digitalWrite(bluePin,LOW);
      digitalWrite(redPin,LOW);
    }

    else{
      digitalWrite(greenPin,LOW);
      digitalWrite(bluePin,LOW);
      digitalWrite(redPin,LOW);
    }
  }

  //Test LEDs
  //digitalWrite(greenPin,HIGH);
  //delay(500);
  //digitalWrite(greenPin,LOW);
  //delay(500);
}
