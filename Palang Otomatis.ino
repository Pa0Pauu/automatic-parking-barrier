#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo Servo1;
Servo Servo2;
int Trig = 12;
int Echo = 13;
int statusTerakhir = -1;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_SERVO1 = 11;
const int PIN_SERVO2 = 5;

void setup() {
  Servo1.attach(PIN_SERVO1);
  Servo2.attach(PIN_SERVO2);
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Servo1.write(0);
  Servo2.write(0);
}

void loop() {
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  unsigned long durasi = pulseIn(Echo, HIGH, 30000);
  float jarak = 0.0343 * durasi / 2;

  int statusSekarang;

  if (jarak > 15) {
    statusSekarang = 0;
  } else if (jarak <= 17) {
    statusSekarang = 1;
  } else {
    statusSekarang = -1;
  }
  if (statusSekarang != statusTerakhir) {
    lcd.clear();

    if (statusSekarang == 0) {
      lcd.setCursor(0, 0);
      lcd.print("CLOSE GATE...");
      lcd.setCursor(0, 1);
      for(int pos=75; pos >= 0; pos--) {
        Servo1.write(pos);
        delay(10);
      }
      Serial.println("TIDAK ADA");
    } 
    else if (statusSekarang == 1) {
      lcd.setCursor(0, 0);
      lcd.print("MEMBUKA...");
      lcd.setCursor(0, 1);
      for(int pos=0; pos <= 75; pos++) {
        Servo1.write(pos);
        delay(10);
      }
      lcd.print("TERBUKA!");
      delay(500);
      Serial.println(jarak);
    } 
    else {
      lcd.setCursor(0, 0);
      lcd.print("MENUNGGU...");
      Servo1.write(0);
    }

    statusTerakhir = statusSekarang;
  }

  delay(100);
}