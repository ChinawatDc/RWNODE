#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
//////////////////
#define relay 27
#define waterlevel 13
#define button_mode_1 12
#define button_mode_2 14
//////////////////
int n_loop = 0;
int time_delay[2] = {0,0};
//int read_time[2] = {5,2};
int time1 = 0;
int time2 = 0;
int cooldown = 0;
int set_cooldown = 0;

///////////////////
int mode_S = 0;
char mode_val = '0';
///////////////////
RF24 radio(16, 15);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;  // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
//////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //NRF24l01
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  //Relay
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
//  //Waterlevel
  pinMode(waterlevel, INPUT_PULLUP);
  /////Button
  pinMode(button_mode_1, INPUT_PULLUP);
  pinMode(button_mode_2, INPUT_PULLUP);
  //SHOWMODE
  Pr_line();
  Serial.print("MODE : ");
  Serial.println(mode_S);
  Pr_line();
}

void loop() {


//Settime
time_delay[0] = (read_time[0] *1000);
time_delay[1] = (read_time[1] *1000);
set_cooldown = cooldown * 1000;

//setmode
//unsigned long buttonState;
//unsigned long buttonState;
if(digitalRead(button_mode_1) == LOW){
  mode_S = 1;
  Serial.print("MODE : ");
  Serial.println(mode_S);
  Pr_line();
  }
else if(digitalRead(button_mode_2) == LOW){
  mode_S = 2;
  Serial.print("MODE : ");
  Serial.println(mode_S);
  Pr_line();
  }
//if(Serial.available()> 0){
//    mode_val = Serial.read();
//    if(mode_val == '1'){
//      mode_S = 1;
//      Serial.print("MODE : ");
//      Serial.println(mode_S);
//      Pr_line();
//    }
//    if(mode_val == '2'){
//      mode_S = 2;
//      Serial.print("MODE : ");
//      Serial.println(mode_S);
//      Pr_line();
//    }
//}
 network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header2;
    unsigned int buttonState;
    network.read(header2, &buttonState, sizeof(buttonState)); // Read the incoming data
//    if(buttonState == LOW){
//      digitalWrite(relay, buttonState);
//    }
//    else{
//      digitalWrite(relay, buttonState);
//    }
  if (mode_S == 1){
    if(buttonState == LOW){
      digitalWrite(relay, buttonState);
    }
    else{
      digitalWrite(relay, buttonState);
    }
  }
  if (mode_S == 2){
    if(buttonState == LOW){
      
      digitalWrite(relay, LOW);
      delay(time_delay[0]);
      digitalWrite(relay, HIGH);
      delay(set_cooldown);
      digitalWrite(relay, LOW);
      delay(time_delay[1]);
      digitalWrite(relay, HIGH);
      delay(set_cooldown);
      set_relay();
    }
    else{
      digitalWrite(relay, HIGH);
    }
  }
}

  //===== Sending =====//
//  //waterlevel
//  unsigned int waterlevelV = digitalRead(waterlevel); // Read waterlevel sensor
//  RF24NetworkHeader header(master00);
//  if(waterlevelV == LOW){
//  bool ok = network.write(header, &waterlevelV, sizeof(waterlevelV)); // Send the data
//  }
//      if(waterlevelV == LOW){
//          Serial.print("WATERLEVEL : ");
//          Serial.println("NOT WATER");
//      }
  
//  digitalWrite(Relay_1,HIGH);
//  delay(2000);
//  digitalWrite(Relay_1,LOW);
//  delay(time_delay[0]);
//  digitalWrite(Relay_1,HIGH);
//  delay(2000);
//  digitalWrite(Relay_1,LOW);
//  delay(time_delay[1]);
}
//void mode_1(){
// digitalWrite(relay, buttonState);
//}
//void mode_2(){
//    digitalWrite(relay, LOW);
//    delay(time_delay[0]);
//    digitalWrite(relay, HIGH);
//    delay(set_cooldown);
//    digitalWrite(relay, LOW);
//    delay(time_delay[1]);
//    digitalWrite(relay, HIGH);
//    delay(set_cooldown);
//}
void Pr_line(){
  Serial.println("+++++++++++");
}
void set_relay(){
  time_delay[0] =0;
  time_delay[1] = 0;
}
