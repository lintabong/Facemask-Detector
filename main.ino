#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);


Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SoftwareSerial mySerial(6,7);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int inc = 0;
float temperature;
uint8_t p;
bool stat_serial;

void setup(){
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  mlx.begin();
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    lcd.setCursor(0,0);
    lcd.print("fingerprint");
    lcd.setCursor(0,1);
    lcd.print("ready");
  } else {
    lcd.setCursor(0,0);
    lcd.print("finger gagal");
    while (1) { 
      delay(1000); 
    }
  }
  
  lcd.clear();
  finger.getParameters();
  finger.getTemplateCount();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("data finger: ");
  lcd.setCursor(14,0);
  lcd.print(finger.templateCount);
  delay(500);
  lcd.clear();
}

void loop(){
  temperature = mlx.readObjectTempC();
  if (Serial.available() > 0){
    inc = Serial.read();
    if (inc == 66) {
      lcd.setCursor(0,0);
      lcd.print("Masukkan Jari    ");
      stat_serial = true;
    } else if (inc == 65){
      lcd.setCursor(0,0);
      lcd.print("Pakai Masker     ");
      stat_serial = false;
    }
  }

  if (stat_serial == true) && (temperature <= 35.00){
    getFingerprintIDez();
    Serial.println("aa");
  } 

  lcd.setCursor(0,1);
  lcd.print("suhu: ");
  lcd.setCursor(6,1);
  lcd.print(temperature);
  
  delay(100);
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  Serial.print("&");
  Serial.print(finger.fingerID);
  Serial.print("&");
  Serial.println(temperature);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("terimakasih");
  delay(2000);
  lcd.clear();
  return finger.fingerID;
}
