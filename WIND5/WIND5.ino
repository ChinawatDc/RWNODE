//More Information at: https://www.aeq-web.com/
//Version 2.0 | 11-NOV-2020
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


//PIN&TIME
const int RecordTime = 3; //Define Measuring Time (Seconds)
const int SensorPin = 13;  //Define Interrupt Pin 13
#define led 5
//VARIABLE
int InterruptCounter;
int level =0;
float WindSpeed;

void setup()
{
  Serial.begin(115200);
  pinMode(12, OUTPUT); //setup LED pin to signal high wind alarm condition
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(2, 0);
  lcd.print("WIND SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop() {
  meassure();
//  Serial.print("Wind Speed: ");
//  Serial.print(WindSpeed);       //Speed in km/h
//  Serial.print(" km/h - ");
//  Serial.print(WindSpeed / 3.6); //Speed in m/s
//  Serial.println(" m/s");
  /////Km/h
//  lcd.setCursor(0, 0);  
//  lcd.print(WindSpeed);  
//  lcd.print("Km/h");
  /////m/s
  lcd.setCursor(0, 0);
  lcd.print(WindSpeed / 3.6);
  lcd.print("m/s ");
  /////ระดับความเร็วลม
  if(WindSpeed == 0){
    Serial.print("Not Working ");
    level=0;
    show_level();
    
  }
  else {
    Serial.print("Working ");
    if((WindSpeed / 3.6) <= 1.5){
      level=1;
      show_level();
    }
    else if((WindSpeed / 3.6) <= 3){
      level=2;
      show_level();
    }
    else if((WindSpeed / 3.6) <= 4.5){
      level=3;
      show_level();
    }
    else if((WindSpeed / 3.6) <= 6){
      level=4;
      show_level();
    }
    else if((WindSpeed / 3.6) <= 7.5){
      level=5;
      show_level();
    }
    else if((WindSpeed / 3.6) <= 9){
      level=6;
      show_level();
    }

    
    if(WindSpeed >= 30){
      digitalWrite(led,HIGH);
    }
    else{
      digitalWrite(led,LOW);
    }
  }
}

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
void show_level(){
  lcd.setCursor(0, 1);
  lcd.print(level);
}
