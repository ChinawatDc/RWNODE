/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
            == Node 012 (child of Node 02)==    
*/
//Library
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//Pin port
#define relay 2
#define waterlevel 12
#define waterflow  13


//Variable
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;

RF24 radio(16, 15);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;  // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format

//water flow
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(1, 0);
  lcd.print("WATER SYSTEM");
  delay(3000);
  lcd.clear();
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  //Relay
  pinMode(relay, OUTPUT);
  //Waterlevel
  pinMode(waterlevel, INPUT_PULLUP);
  //Waterflow
  pinMode(waterflow, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(waterflow), pulseCounter, FALLING);
}

void loop() {
//  lcd.setCursor(0, 0);
//  lcd.print("Pump");
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long buttonState;
    network.read(header, &buttonState, sizeof(buttonState)); // Read the incoming data
    digitalWrite(relay, !buttonState); // Turn on or off the RELAY
//    lcd.setCursor(5, 0);
//    lcd.print(buttonState );
  }
  //===== Sending =====//
  //waterlevel
  unsigned long waterlevelV = digitalRead(waterlevel); // Read waterlevel sensor
  RF24NetworkHeader header8(master00);
  bool ok = network.write(header8, &waterlevelV, sizeof(waterlevelV)); // Send the data
  lcd.setCursor(0, 1);
  lcd.print("WaterLevel:");
  if(waterlevelV == HIGH){
      lcd.setCursor(12, 1);
      lcd.print("HIGH ");
  }
  else{
    lcd.setCursor(12, 1);
    lcd.print("LOW ");
  }
  //water flow
  lcd.setCursor(0, 0);
  lcd.print("WaterFlow:");
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
  
  }
  unsigned long waterflowS = int(flowRate); 
  RF24NetworkHeader header9(master00);
  bool ok2 = network.write(header9, &waterflowS, sizeof(waterflowS)); // Send the data
  lcd.setCursor(11, 0);
  lcd.print(waterflowS);
  lcd.print(" L/m");
  
}
