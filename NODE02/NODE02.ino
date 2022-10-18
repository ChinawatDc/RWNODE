/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//PIN&TIME
const int RecordTime = 3; //Define Measuring Time (Seconds)
const int SensorPin = 13;  //Define Interrupt Pin 13
#define led 5
//VARIABLE
int InterruptCounter;
int level =0;
float WindSpeed;
//float sum[10] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.9};
//int i = 0;


RF24 radio(16, 15);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(led, OUTPUT); //setup LED pin
}
void loop() {
  meassure();
  
  network.update();
  //===== Sending =====//
  //unsigned int WindSpeed_S = sum[i];
  unsigned int WindSpeed_S = WindSpeed;
  Serial.print("WindSpeed_S : ");
  Serial.println(WindSpeed_S);
  RF24NetworkHeader header(master00);   // (Address where the data is going)
  bool ok = network.write(header, &WindSpeed_S, sizeof(WindSpeed_S)); // Send the data
  //loop_sum();
  delay(500);
}
//void loop_sum(){
//  i++;
//  if(i == 10){
//    i = 0;
//  }
//}
void meassure() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));
  WindSpeed = (float)InterruptCounter / ((float)RecordTime * 3.6);
}
void countup() {
  InterruptCounter++;
}
