/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include "Keypad.h"
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
#define VBATPIN A9

#define garageRelay A0
#define garageMagnetStatus A5
/*=========================================================================*/

unsigned long lastConnectionTime = 0;            // last time we connected to the server, in milliseconds
const unsigned long postingInterval = 30L * 1000L; // delay between updates, in milliseconds

//Specify the Code on the Key Pad you want
char* secretCode = "1234#";
int position = 0;
bool lastGarageState = LOW;
bool isGarageOpen = LOW;

//Iniliziase the KeyPad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 6, 5}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//DHT dht(DHTPIN, DHTTYPE);

// Create the bluefruit object
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void)
{
  pinMode(garageRelay, OUTPUT);
  pinMode(garageMagnetStatus, INPUT);
  
  Serial.begin(115200);

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  if ( FACTORYRESET_ENABLE )
  {
  if ( ! ble.factoryReset() ){
    error(F("Couldn't factory reset"));
  }
  }
  ble.echo(false);
  ble.info();
  //Set BLE Name
  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=BLE-Garage")) ) {
    Serial.println(F("Failed to set name"));
  }
  //Set BLE Powerlevel
  ble.sendCommandCheckOK("AT+BLEPOWERLEVEL=4");
  ble.verbose(false);
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }
  
  //Serial.println( F("Switching to DATA mode!") );
  //ble.setMode(BLUEFRUIT_MODE_DATA);
}
void loop(void)
{
  pinMode(garageMagnetStatus, INPUT_PULLUP); 

  lastGarageState = isGarageOpen;
  isGarageOpen = digitalRead(garageMagnetStatus);

  if(lastGarageState != isGarageOpen){
      delay(1000);
      //Serial.println("Status Different");
     if(isGarageOpen == LOW){
      Serial.println("OPEN");
        sendStatusToBle(HIGH);
     }else{
      Serial.println("CLOSED");
        sendStatusToBle(LOW);
     }
  };
  //Check if KeyPad was pressed
  char key = keypad.getKey();
   
  if (key == '*')
  {
     position = 0;
     Serial.print("Reset at Position:");Serial.println("Reset at Position:"+position);      
  }
  if (key != 0 )
  {
      Serial.print("Key pressed:"); 
      Serial.println(key); 
     if (key == secretCode[position])  // Valid key in Password sequence
     {
        Serial.print("Key pressed:"); 
        Serial.println(key);   
        Serial.print("Matched ");   
        Serial.print(key);   
        Serial.print("-at-");   
        Serial.println(position);   
        position +=1;
     }
     else  // Invalid key - start all over again
     {
        Serial.println("Invalid Code!");   
        position = 0;
     }
  }
  if (position == 5)  // Password successfully entered - Open or Close Garage
  {
    Serial.println("Positon is 4, Correct code");   
    position = 0;
    digitalWrite(garageRelay, HIGH);
    delay(500);
    digitalWrite(garageRelay, LOW);
//    if(isGarageOpen == LOW){
//        sendStatusToBle(HIGH);
//     }else{
//        sendStatusToBle(LOW);
//    }
  }

  // Get received data from Ble
  getBleIncommingData();
}

void getBleIncommingData(){
  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }
  
  String bleBuffer = ble.buffer;
  Serial.print(F("[Recv] ")); 
  Serial.println(bleBuffer);

  int midColon = bleBuffer.indexOf(':');
  int stringLenght = bleBuffer.length();
  
  String command = bleBuffer.substring(0,midColon);
  Serial.println(command);
  if (command == "GarageDoor") {
      Serial.println("Command GarageDoor");
      digitalWrite(garageRelay, HIGH);
      delay(500);
      digitalWrite(garageRelay, LOW);
  };
  ble.waitForOK();
}

void sendStatusToBle(bool garageStatus){
  if(ble.isConnected()){    
    //Send Temperature value from AnalogPin out on BLE
    ble.print("AT+BLEUARTTX=");
    if(garageStatus == LOW){
      ble.println("{GarageDoor:Down}");
    }else{
      ble.println("{GarageDoor:Up}");
    }
    delay(1000);
    ble.waitForOK();
  }  
}
