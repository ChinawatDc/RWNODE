/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
/////ประกาศเรียกใช้
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

/////PORT
#define button 13
/////ตัวแปร
char val = '0';
int buttonState_level_S = 0;
/////NRF24L01
RF24 radio(4, 5);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node011 = 011;
//const uint16_t node022 = 022;

void setup() {
  Serial.begin(9600);
  /////NRF24L01
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  /////Button
  pinMode(button, INPUT_PULLUP);
  //pinMode(led, OUTPUT);
}

void loop() {
/////การส่งค่าระดับความแรงการฉีดพ่น
 if (Serial.available()> 0)
  {
    val = Serial.read();
    Serial.print("Val : ");
    Serial.println(val);
    Serial.println("+++++++++++");
  }
   if (val == '1'){
      buttonState_level_S = 1;
    }
   if (val == '2'){
      buttonState_level_S = 2;
    }
   if (val == '3'){
      buttonState_level_S = 3;
    }

  network.update();
  //===== รับค่า =====//
//  while ( network.available() ) {     // Is there any incoming data?
//    RF24NetworkHeader header;
//    unsigned long waterlevelV;
//    network.read(header, &waterlevelV, sizeof(waterlevelV)); // Read the incoming data
//    Serial.print("WATERLEVEL : ");
//    //if (header.from_node == 01) {    // If data comes from Node 01
//      if(waterlevelV == HIGH){
//          Serial.println("NOT WATER");
//      }
//      else{
//          Serial.println("HAVE WATER");
//      }
    //}
////    if (header.from_node == 02) {    // If data comes from Node 02
////      digitalWrite(led, !incomingData); 
////    }
//    
//    //RF24NetworkHeader header;
////    unsigned long waterflowS;
////    network.read(header, &waterflowS, sizeof(waterflowS)); // Read the incoming data
////    lcd.setCursor(11, 1);
////    lcd.print(waterflowS);
////    lcd.print(" L/m");
//  }
  //===== ส่งค่า =====//
  //  // Servo control at Node 01
  //  unsigned long potValue = analogRead(A0);
  //  unsigned long angleValue = map(potValue, 0, 1023, 0, 180); // Suitable for servo control
  //  RF24NetworkHeader header2(node01);     // (Address where the data is going)
  //  bool ok = network.write(header2, &angleValue, sizeof(angleValue)); // Send the data

  // ส่งค่าปุ่มกด Control at Node 01
  unsigned long buttonState = digitalRead(button);
  RF24NetworkHeader header2(node01);    // (Address where the data is going)
  bool ok = network.write(header2, &buttonState, sizeof(buttonState)); // Send the data
//  Serial.print("buttonState : ");
//  Serial.println(buttonState);

//  lcd.setCursor(0, 1);
//  lcd.print("ButtonState");
//  lcd.setCursor(12, 1);
//  lcd.print(buttonState);
  // ส่งค่าระดับความแรงน้ำ control at Node 011
  unsigned long buttonState_level = 0;
  if(buttonState_level_S > 0){
    buttonState_level = buttonState_level_S;
    }
//    Serial.print("buttonState_level : ");
//    Serial.println(buttonState_level);
  RF24NetworkHeader header4(node011);    // (Address where the data is going)
  bool ok3 = network.write(header4, &buttonState_level, sizeof(buttonState_level)); // Send the data
}
