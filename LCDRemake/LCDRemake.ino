#include <LiquidCrystal.h>
#define RS 0
#define EN 1
#define D4 2
#define D5 3
#define D6 4
#define D7 5

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

byte fumoEye[] = {
  B00000, B00000, B11111, B10001,
  B10001, B10001, B01110, B00000
};

byte fumoMouth[] = {
  B00000, B00000, B00000, B00000,
  B00000, B00100, B01010, B00000
};

byte arioSimbolo[] = {
  B00000, B00000, B00110, B10100,
  B11111, B00101, B01100, B00000
};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, fumoEye);
  lcd.createChar(1, fumoMouth);
  lcd.createChar(2, arioSimbolo);
  lcd.home();
}

void loop() {
  lcd.home();
  lcd.clear();
  nombreMau();
  nombreNolazco();
}

void nombreMau() {
  lcd.setCursor(0, 0);
  lcd.print("Mauricio ");
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(0));
}

void nombreNolazco() {
  lcd.setCursor(2, 1);
  lcd.write(byte(2));
  lcd.print(" Nolazco ");
  lcd.write(byte(2));
}
