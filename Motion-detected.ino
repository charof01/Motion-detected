#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// กำหนดขา I2C สำหรับ ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// กำหนดที่อยู่ I2C ของจอ LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// กำหนดขาของ PIR sensor และ Buzzer
const int pirData = 15;  // ขาของ PIR sensor
const int buzzerPin = 12; // ขาของ Buzzer

int pirState = LOW;      // สถานะ PIR
int value = 0;           // ค่าที่อ่านจาก PIR sensor

// ความถี่สำหรับไซเรน
const int sirenHigh = 1000;  // ความถี่สูง
const int sirenLow = 400;    // ความถี่ต่ำ
const int sirenDuration = 200; // ระยะเวลาแต่ละโน้ต
const int sirenPause = 100;   // ระยะเวลาหยุด

void playPoliceSiren() {
  for (int i = 0; i < 10; i++) {   // เล่นเสียงไซเรน 10 ครั้ง
    tone(buzzerPin, sirenHigh);     // เล่นเสียงสูง
    delay(sirenDuration);           // เล่นเสียงเป็นระยะเวลา
    noTone(buzzerPin);              // หยุดเสียง
    delay(sirenPause);              // หยุดชั่วขณะ

    tone(buzzerPin, sirenLow);      // เล่นเสียงต่ำ
    delay(sirenDuration);           // เล่นเสียงเป็นระยะเวลา
    noTone(buzzerPin);              // หยุดเสียง
    delay(sirenPause);              // หยุดชั่วขณะ
  }
}

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(pirData, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // ปิด Buzzer

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 Motion");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
  Serial.begin(115200);
}

void loop() {
  value = digitalRead(pirData);

  if (value == HIGH) {
    if (pirState == LOW) {
      playPoliceSiren();  // เล่นเสียงไซเรน

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DANGER ALERT!");
      lcd.setCursor(0, 1);
      lcd.print("Motion Detected!");
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      noTone(buzzerPin); // ปิดเสียง Buzzer

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Motion");
      lcd.setCursor(0, 1);
      lcd.print("Waiting...");
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}
