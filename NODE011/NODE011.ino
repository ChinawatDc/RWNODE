/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
            == Node 012 (child of Node 02)==    
*/
/////ประกาศเรียกใช้
#include <Stepper.h>
#define STEPS 2048 // the number of steps in one revolution of your motor (28BYJ-48)
Stepper stepper(STEPS, 13, 14, 12, 27); // เวลาเสียบกับบอร์ด เรียงตามปกติคือ 2 3 4 5
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
/////PORT
//#define relay 2
//#define IR 3
/////ตัวแปร
int last_level = 1;
/////NRF24L01
RF24 radio(4, 5);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 011;  // Address of our node in Octal format ( 04,031, etc)
//const uint16_t node01 = 01;    // Address of the other node in Octal format
const uint16_t master00 = 00;    // Address of the other node in Octal format

void setup() {
  Serial.begin(9600);
  /////LCD
  lcd.init();
  lcd.backlight();      // Make sure backlight is on
  /////NRF24L01
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  //stepper.setSpeed(10); // เลือกความเร็ว
  //stepper.step(0);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
  //pinMode(relay, OUTPUT);
  //pinMode(IR, INPUT);
}

void loop() {
  /////แสดงระดับและสถานะ
  lcd.setCursor(0, 0);
  lcd.print("Level : ");
  lcd.setCursor(0, 1);
  lcd.print("State : ");
  /////NRF24L01
  network.update();
  //===== รับค่า =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long buttonState_level = 1;
    network.read(header, &buttonState_level, sizeof(buttonState_level)); // Read the incoming data
    //digitalWrite(relay, !buttonState); // Turn on or off the RELAY
    lcd.setCursor(8, 0);
    lcd.print(buttonState_level);
    lcd.print("   ");
    Serial.print("buttonState_level : ");
    Serial.println(buttonState_level);
    if (buttonState_level == 1){
      if(last_level != 1){
        if(last_level == 2){
          level_2_to_1();
          lcd.setCursor(8, 1);
          lcd.print("2 to 1");
          lcd.print("   ");
        }
        else if(last_level == 3){
          level_3_to_1();
          lcd.setCursor(8, 1);
          lcd.print("3 to 1");
          lcd.print("   ");
        }
      }
      last_level=1;
    }
    if (buttonState_level == 2){
    if(last_level != 2){
        if(last_level == 1){
          level_1_to_2();
          lcd.setCursor(8, 1);
          lcd.print("1 to 2");
          lcd.print("   ");
        }
        else if(last_level == 3){
          level_3_to_2();
          lcd.setCursor(8, 1);
          lcd.print("3 to 2");
          lcd.print("   ");
        }
      }
      last_level=2;
    }  
    if (buttonState_level == 3){           
    if(last_level != 3){
        if(last_level == 1){
          level_1_to_3();
          lcd.setCursor(8, 1);
          lcd.print("1 to 3");
          lcd.print("   ");
        }
        else if(last_level == 2){
          level_2_to_3();
          lcd.setCursor(8, 1);
          lcd.print("2 to 3");
          lcd.print("   ");
        }
      }
      last_level=3;
      }
    }
//  //===== Sending =====//
//  unsigned long irV = digitalRead(IR); // Read IR sensor
//  RF24NetworkHeader header8(node01);
//  bool ok = network.write(header8, &irV, sizeof(irV)); // Send the data
}
void level_1_to_2(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(-490);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
void level_1_to_3(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(-880);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
void level_2_to_1(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(490);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
void level_2_to_3(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(-390);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
void level_3_to_1(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(880);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
void level_3_to_2(){
  stepper.setSpeed(10); // เลือกความเร็ว
  stepper.step(390);  //หมุนไปทิศทาง 90 องศา , ถ้า 1024 จะหมุนไปทิศทาง 180 องศา
}
