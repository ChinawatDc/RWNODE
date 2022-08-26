/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define button 2
#define led 3
#define led 4

RF24 radio(16, 15);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node012 = 012;
const uint16_t node022 = 022;

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(2, 0);
  lcd.print("MAIN SYSTEM");
  delay(3000);
  lcd.clear();
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("WaterLevel:");
  lcd.setCursor(0, 1);
  lcd.print("WaterFlow:");
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
//    unsigned long incomingData;
//    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    //analogWrite(led, incomingData);    // PWM output to LED 01 (dimming)
//    unsigned long waterlevelV;
//    network.read(header, &waterlevelV, sizeof(waterlevelV)); // Read the incoming data
//    digitalWrite(led, !waterlevelV);
//    if(waterlevelV == HIGH){
//      lcd.setCursor(12, 0);
//      lcd.print("HIGH ");
//    }
//    else{
//      lcd.setCursor(12, 0);
//      lcd.print("LOW ");
//    }
    //RF24NetworkHeader header;
    unsigned long waterflowS;
    network.read(header, &waterflowS, sizeof(waterflowS)); // Read the incoming data
    lcd.setCursor(11, 1);
    lcd.print(waterflowS);
    lcd.print(" L/m");
  }
  //===== Sending =====//
  //  // Servo control at Node 01
  //  unsigned long potValue = analogRead(A0);
  //  unsigned long angleValue = map(potValue, 0, 1023, 0, 180); // Suitable for servo control
  //  RF24NetworkHeader header2(node01);     // (Address where the data is going)
  //  bool ok = network.write(header2, &angleValue, sizeof(angleValue)); // Send the data

  // LED Control at Node 012
  unsigned long buttonState = digitalRead(button);
  RF24NetworkHeader header4(node01);    // (Address where the data is going)
  bool ok3 = network.write(header4, &buttonState, sizeof(buttonState)); // Send the data
//  lcd.setCursor(0, 1);
//  lcd.print("ButtonState");
//  lcd.setCursor(12, 1);
//  lcd.print(buttonState);
  //  // LEDs control at Node 022
  //  unsigned long pot2Value = analogRead(A1);
  //  RF24NetworkHeader header3(node022);    // (Address where the data is going)
  //  bool ok2 = network.write(header3, &pot2Value, sizeof(pot2Value)); // Send the data
}
