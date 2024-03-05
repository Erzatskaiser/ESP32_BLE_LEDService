//BLE header files
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//Define UUIDs
#define SERVICE_UUID "d934e3ea-4a99-4229-807c-2eeb021e3b7a"
#define RED          "4f2ec411-cdc6-4e42-8705-6221448f3ba3"
#define GREEN        "c34faf8c-a288-4dec-ba29-8bd32a9036c5"
#define BLUE         "3dc94628-3be5-4676-867d-d6a4bd6b5d91"

//BLE Characteristics
BLECharacteristic rLED(RED, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic gLED(GREEN, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic bLED(BLUE, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

//Miscleanous variables
const int maxWidth = 2;
char incomingData [maxWidth];
bool deviceConnected = false;

//Callback for onConnect and onDisconnets
class myServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    deviceConnected = true;
    Serial.println("Device connected");
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

//Initial setup
void setup() {
  //Setup serial connection
  Serial.begin(9600);

  //Initialize BLE server
  BLEDevice::init("LED Service");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new myServerCallbacks());

  //Create and start the BLE service
  BLEService *lService = pServer->createService(SERVICE_UUID);
  lService->addCharacteristic(&rLED);
  lService->addCharacteristic(&gLED);
  lService->addCharacteristic(&bLED);
  lService->start();

  //Start advertising
  pServer->getAdvertising()->start();
  Serial.println("INITIATED DEVICE...");

  //Initialize to all on
  int on = 1;
  rLED.setValue(on);gLED.setValue(on);bLED.setValue(on);
}

void loop() {
  int on = 1; int off = 0;
  //When data comes in from serial
  if(deviceConnected){
    if (Serial.available() > 0) {
        //Read incoming serial data
        Serial.readBytes(incomingData, maxWidth);

        //Print the input for debugging; Numbers located at index [0]
        Serial.print("Data received: ");
        for (int i=0; i<maxWidth; i++){
          Serial.print(incomingData[i]);
        }
        Serial.println("");

        //Transform input into integer
        int input = atoi(incomingData);

        //Based on the input, update characteristic
        if (input==0){
          //Acknowledge
          Serial.println("PROCESSING INPUT...");

          //Update characteristic values
          rLED.setValue(off);gLED.setValue(off);bLED.setValue(off);

          //Notify change in characteristic value
          rLED.notify();gLED.notify();bLED.notify();

          //Print the value received
          Serial.print("Data update: ");
          Serial.println(incomingData[0]);
        }
        else if (input==1){
          //Acknowledge
          Serial.println("PROCESSING INPUT...");

          //Update characteristic values
          rLED.setValue(on);gLED.setValue(off);bLED.setValue(on);

          //Notify change in characteristic value
          rLED.notify();gLED.notify();bLED.notify();

          //Print the value received
          Serial.print("Data update: ");
          Serial.println(incomingData[0]);
        }
        else if (input==2){
          //Acknowledge
          Serial.println("PROCESSING INPUT...");

          //Update characteristic values
          rLED.setValue(off);gLED.setValue(on);bLED.setValue(off);

          //Notify change in characteristic value
          rLED.notify();gLED.notify();bLED.notify();

          //Print the value received
          Serial.print("Data update: ");
          Serial.println(incomingData[0]);
        }
        else if (input==3){
          //Acknowledge
          Serial.println("PROCESSING INPUT...");

          //Update characteristic values
          rLED.setValue(off);gLED.setValue(off);bLED.setValue(on);

          //Notify change in characteristic value
          rLED.notify();gLED.notify();bLED.notify();

          //Print the value received
          Serial.print("Data update: ");
          Serial.println(incomingData[0]);
        }
        else if (input==4){
          //Acknowledge
          Serial.println("PROCESSING INPUT...");

          //Update characteristic values
          rLED.setValue(on);gLED.setValue(on);bLED.setValue(on);

          //Notify change in characteristic value
          rLED.notify();gLED.notify();bLED.notify();

          //Print the value received
          Serial.print("Data update: ");
          Serial.println(incomingData[0]);
        }
      }
  }
}
