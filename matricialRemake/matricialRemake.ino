#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>
#define ROWS 4
#define COLS 4
#define RS 13
#define EN 12
#define D4 8
#define D5 9
#define D6 10
#define D7 11

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
long int lastMS;
char keys[ROWS][COLS] =  {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const uint8_t rowsPins[ROWS] = { 0, 1, 2, 3 };
const uint8_t colsPins[COLS] = { 4, 5, 6, 7 };
Keypad kpad = Keypad(makeKeymap(keys), rowsPins, colsPins, ROWS, COLS);

void setup() {
  lcd.begin(16, 2);
  lcd.home();
  lastMS = 0;
}

void loop() {
  if ((millis() - lastMS) > 1000/20) {
    char k = kpad.getKey();
    if (k != NULL)
      EEPROM.write(0, k);
    lcd.home();
    lcd.print((char)EEPROM.read(0));
    lastMS = millis();
  }
}
