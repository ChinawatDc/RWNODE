#include <EEPROM.h>
#include <Arduino.h>
#define windDir A0 /// ทิศาทงลม
#define ANEMOMETER D7 // Interrupt pin ความเร็วลม
#define DEBOUNCE_TIME 15
#define CALC_INTERVAL 1000
const int Rain = D6;    /// Interrupt pin วัดปริมาณน้ำฝน  
const int button = D3;   /// Interrupt pin Reset น้ำฝน
unsigned long nextCalc;
unsigned long timer;
volatile int anemometerCounter;
volatile unsigned long last_micros_an;
int windSpd;
int sensorExp[] = {66,84,93,126,184,244,287,406,461,599,630,702,785,827,886,945};
float dirDeg[] = {112.5,67.5,90,157.5,135,202.5,180,22.5,45,247.5,225,337.5,0,292.5,315,270};
char* dirCard[] = {"ESE","ENE","E","SSE","SE","SSW","S","NNE","NE","WSW","SW","NNW","N","WNW","NW","W"};
int sensorMin[] = {63,80,89,120,175,232,273,385,438,569,613,667,746,812,869,931};
int sensorMax[] = {69,88,98,133,194,257,301,426,484,612,661,737,811,868,930,993};
int incoming = 0;
float angle = 0;
char* dir = "nan";
volatile short counter_Rain ; 
ICACHE_RAM_ATTR void detects() {
counter_Rain++;
Serial.print((float)counter_Rain);Serial.println(" mm");
writeEEPROM(counter_Rain);
}
ICACHE_RAM_ATTR void resetEEPROM() {
Serial.println("Reset EEPROM ");
writeEEPROM(0);
counter_Rain = readEEPROM();
ESP.restart(); 
 }
void writeEEPROM(unsigned short number)
{
 EEPROM.write(0, number >> 8);
 EEPROM.write(1, number & 0xFF);
 EEPROM.commit();
}
unsigned short readEEPROM()
{
 byte dataHigh = EEPROM.read(0);
 byte dataLow = EEPROM.read(1);
  return (dataHigh << 8) + dataLow;
}
void setup() 
{
Serial.begin(9600); 
pinMode( Rain , INPUT_PULLUP);
pinMode( button , INPUT_PULLUP);
pinMode(ANEMOMETER , INPUT_PULLUP);
EEPROM.begin(512);
counter_Rain = readEEPROM();
attachInterrupt(digitalPinToInterrupt(ANEMOMETER), countAnemometer, FALLING); 
attachInterrupt(digitalPinToInterrupt(counter_Rain), detects, FALLING); 
attachInterrupt(digitalPinToInterrupt(button), resetEEPROM, FALLING);
}
void loop() 
 {
Serial.print("Dir:"),Serial.println(dir);
Serial.print("Rain: ");Serial.print((float) counter_Rain);Serial.println(" mm.");  
Serial.print("WindSpd:");Serial.print(windSpd);Serial.println(" km/h");
delay(1000);
 incoming = analogRead(windDir);
  for(int i=0; i<=15; i++) {
  if(incoming >= sensorMin[i] && incoming <= sensorMax[i]) {
  dir = dirCard[i];
  break;
   } 
  }
 timer = millis();
   if(timer > nextCalc) {
  nextCalc = timer + CALC_INTERVAL;
  windSpd = readWindSpd();
  }
  }
 int readWindSpd() {
 unsigned char i;  
 long spd = 14920; 
 spd *= anemometerCounter;
 spd /= 10000;
anemometerCounter = 0;
 return (int) spd;
}
ICACHE_RAM_ATTR void countAnemometer() {
 if((long)(micros() - last_micros_an) >= DEBOUNCE_TIME * 1000) {
    anemometerCounter++;
    last_micros_an = micros();
 }
}
