The above scripts draw from the following tutorials:          
https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/          
https://randomnerdtutorials.com/esp32-ble-server-client/

The master and slave code both compile and upload to the boards fine. Furthermore, the master device can be connected to from a mobile device, where one can observe that inputting data into the Arduino IDE's serial monitor does indeed update the characteristics of the BLE service. The issue arises when attempting to connect the two, which yields the error: "Guru Meditation Error: Core  0 panic'ed (IllegalInstruction). Exception was unhandled." . I think that this may be due to the connection sequence being implemented incorrectly in my script, but still need to review this further.
