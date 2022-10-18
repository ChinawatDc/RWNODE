/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
/////ประกาศเรียกใช้
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

/////WIFI&MQTT
#define WIFI_STA_NAME "TGR28"
#define WIFI_STA_PASS "TGR_0000"

#define MQTT_SERVER   "54.191.199.50"
#define MQTT_PORT     1883
#define MQTT_USERNAME "phusit"
#define MQTT_PASSWORD "Password123#@!"
#define MQTT_NAME     "ESP32_1"
#define Topic1 "client/mcu02"
#define Topic2 "client/mcu01"

WiFiClient client;
PubSubClient mqtt(client);

/////PORT
#define button 13
#define button_L1 12
#define button_L2 14
#define button_L3 27
/////ตัวแปร
char val = '0';
int buttonState_level_S = 0;
int level =0;
float windspeed = 0;
int waterlevel ;
int buttonS = 1;
/////NRF24L01
RF24 radio(4, 5);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node011 = 011;
//const uint16_t node022 = 022;

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char*)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);
  Serial.println();
  // Feel free to add more if statements to control more GPIOs with MQTT
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "client/mcu01") {
    buttonS = HIGH;
  }
  else if (String(topic) == "client/mcu02") {
   buttonS = LOW;
  }
}
void setup() {
  Serial.begin(9600);
  /////LCD
  lcd.init();
  lcd.backlight();
  /////NRF24L01
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  /////Button
  pinMode(button, INPUT_PULLUP);
  pinMode(button_L1, INPUT_PULLUP);
  pinMode(button_L2, INPUT_PULLUP);
  pinMode(button_L3, INPUT_PULLUP);
  //pinMode(led, OUTPUT);
  //oled
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  //MQTT
    Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}

void loop() {
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe(Topic1);
      mqtt.subscribe(Topic2);
    } 
    else {
      Serial.println("failed");
      delay(5000);
    }
  } 
  else {
    mqtt.loop();
  }
/////การส่งค่าระดับความแรงการฉีดพ่น
// if (Serial.available()> 0)
//  {
//    val = Serial.read();
//    Serial.print("Val : ");
//    Serial.println(val);
//    Serial.println("+++++++++++");
//  }
  if(digitalRead(button_L1) == LOW){
    buttonState_level_S = 1;
    Pr_level();
    Serial.println(buttonState_level_S);
    Pr_line();
    }
  else if(digitalRead(button_L2) == LOW){
    buttonState_level_S = 2;
    Pr_level();
    Serial.println(buttonState_level_S);
    Pr_line();
    }
  else if(digitalRead(button_L3) == LOW){
    buttonState_level_S = 3;
    Pr_level();
    Serial.println(buttonState_level_S);
    Pr_line();
    }
//   if (val == '1'){
//      buttonState_level_S = 1;
//    }
//   if (val == '2'){
//      buttonState_level_S = 2;
//    }
//   if (val == '3'){
//      buttonState_level_S = 3;
//    }
  /////แสดงระดับและสถานะ
  lcd.setCursor(0, 0);
  lcd.print("Speed : ");
  lcd.setCursor(0, 1);
  lcd.print("Level : ");
  network.update();
  //===== รับค่า =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned int incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    if (header.from_node == 9) {    // If data comes from Node 01
      waterlevel = incomingData;
      if(waterlevel == LOW){
          Serial.print("WATERLEVEL : ");
          Serial.println("NOT WATER");
      }
//      else{
//          Serial.println("HAVE WATER");
//      }
    }
    if(header.from_node == 2){
      windspeed = incomingData;
      Serial.print("Wind Speed: ");
      Serial.print(windspeed);       //Speed in km/h
      Serial.println(" km/h");
      
      lcd.setCursor(8, 0);
      lcd.print(windspeed+(windspeed/3.1));
      lcd.print("   ");
      if(windspeed == 0){
        Serial.println("Not Working ");
        level=0;
        Pr_line();
        show_level();
        Serial.println(level);
        lcd.setCursor(8, 1);
        lcd.print(level);
        lcd.print("   ");
      }
      else {
        Serial.println("Working ");
        if((windspeed / 3.6) <= 1.5){
          level=1;
          Pr_line();
          show_level();
          Serial.println(level);
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
        else if((windspeed / 3.6) <= 3){
          level=2;
          Pr_line();
          show_level();
          Serial.println(level);
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
        else if((windspeed / 3.6) <= 4.5){
          level=3;
          Pr_line();
          show_level();
          Serial.println(level);
          Pr_line();
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
        else if((windspeed / 3.6) <= 6){
          level=4;
          Pr_line();
          show_level();
          Serial.println(level);
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
        else if((windspeed / 3.6) <= 7.5){
          level=5;
          Pr_line();
          show_level();
          Serial.println(level);
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
        else if((windspeed / 3.6) <= 9){
          level=6;
          Pr_line();
          show_level();
          Serial.println(level);
          lcd.setCursor(8, 1);
          lcd.print(level);
          lcd.print("   ");
        }
  
      
        if(windspeed >= 30){
          //digitalWrite(led,HIGH);
        }
        else{
          //digitalWrite(led,LOW);
        }
      }
     }
   }
////    if (header.from_node == 02) {    // If data comes from Node 02
////      digitalWrite(led, !incomingData); 
////    }   
  //===== ส่งค่า =====//
  //  // Servo control at Node 01
  //  unsigned long potValue = analogRead(A0);
  //  unsigned long angleValue = map(potValue, 0, 1023, 0, 180); // Suitable for servo control
  //  RF24NetworkHeader header2(node01);     // (Address where the data is going)
  //  bool ok = network.write(header2, &angleValue, sizeof(angleValue)); // Send the data

  // ส่งค่าปุ่มกด Control at Node 01
  unsigned int buttonState;
  if(digitalRead(button) == HIGH){
    buttonState = buttonS;
  }
  else{
    buttonState = digitalRead(button);
  }
  RF24NetworkHeader header2(node01);    // (Address where the data is going)
  bool ok = network.write(header2, &buttonState, sizeof(buttonState)); // Send the data
//  Serial.print("State : ");
//  Serial.println(buttonState);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.print("StatePump:");
  if(buttonState == 0){
    display.setCursor(65, 5);
    display.print("Pump ON");
  }
  else{
    display.setCursor(65, 5);
    display.print("Pump OFF");
  }
    Pr_line();
    delay(100);
  // ส่งค่าระดับความแรงน้ำ control at Node 011
  unsigned int buttonState_level = 0;
  if(buttonState_level_S > 0){
    buttonState_level = buttonState_level_S;
    }
  display.setCursor(0, 20);
  display.print("LevelPump:");
  if(buttonState_level > 0){
    display.setCursor(65, 20);
    display.print(buttonState_level);
  }
  else{
    display.setCursor(65, 20);
    display.print("Wait.... ");
  }
  display.setCursor(0, 35);
  display.print("StateWater:");
  if(waterlevel == 0){
    display.setCursor(65, 35);
    display.print("Not Have");
  }
  else{
    display.setCursor(65, 35);
    display.print("Have Water");
  } 
  display.setCursor(0, 50);
  display.print("windspeed:");
  display.setCursor(65, 50);
  display.print(windspeed+(windspeed/3.1));
  display.print(" km/h");
//    Serial.print("buttonState_level : ");
//    Serial.println(buttonState_level);
  RF24NetworkHeader header4(node011);    // (Address where the data is going)
  if(digitalRead(button_L1)==LOW||digitalRead(button_L2)==LOW||digitalRead(button_L3)==LOW)
  bool ok3 = network.write(header4, &buttonState_level, sizeof(buttonState_level)); // Send the data
display.display();
}
void Pr_line(){
  Serial.println("+++++++++++");
}
void Pr_level(){
  Serial.print("Level : ");
}
void show_level(){
  Serial.print("WindLevel : ");
}
