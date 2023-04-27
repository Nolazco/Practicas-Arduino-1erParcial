#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>
#define PASSW_LOC 271
#define PASSW_SET 270
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

byte selectArrow[] = {
  B00000, B00100, B11110, B11111,
  B11110, B00100, B00000, B00000
};
const uint8_t rowsPins[ROWS] = { 0, 1, 2, 3 };
const uint8_t colsPins[COLS] = { 4, 5, 6, 7 };
Keypad kpad = Keypad(makeKeymap(keys), rowsPins, colsPins, ROWS, COLS);

const char *masterKey = "1984";
void (*screen)() = NULL;
bool openedLock = false;

void setup() {
  DDRC = B00000011;
  PORTC = B00000000;
  lcd.createChar(0, selectArrow);
	lcd.begin(16, 2);
  lcd.home();
  lastMS = 0;
  screen = &type_master_key;
  // EEPROM[PASSW_SET] = false;
}

void set_passw(char new_passw[4]) {
  EEPROM[PASSW_SET] = true;
  for (int i = 0; i < 4; i++) 
    EEPROM[PASSW_LOC + i] = new_passw[i];
  EEPROM[PASSW_LOC + 4] = '\0';
}

bool get_passw(char buffer[5]) {
  if (!EEPROM[PASSW_SET])
    buffer[0] = '\0';
  else
    for (int i = 0; i < 4; i++)
      buffer[i] = EEPROM[PASSW_LOC + i];
  return EEPROM[PASSW_SET];
}

void loop() {
  if ((millis() - lastMS) > 1000/10) {
    lcd.clear();
    lcd.home();
    lastMS = millis();
    if (screen != NULL)
      screen();
  }
}

void incorrect_password() {
  lcd.clear();
  lcd.home(); 
  lcd.print(" V    BAD     V ");
  lcd.setCursor(0, 1);
  lcd.print(" o  PASSWORD  o ");
  PORTC = B00000010;
  tone(16, 500, 1000);
  delay(2000);
  PORTC = B00000000;
}

void master_menu() {
  char k = 0;
  int opt = 0;
  lcd.print("--=> SELECT <=--");
  do {
    lcd.setCursor(0, 1);
    lcd.print(" NEW ");
    lcd.print(openedLock? " CLOSE" : " OPEN");
    switch(opt) {
      case 0:
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        break;
      case 1:
        lcd.setCursor(5, 1);
        lcd.write(byte(0));
        break;
    }
    k = kpad.waitForKey();
    if (k == '*')
      opt = (opt + 1) % 2;
  } while(k != '#');

  screen = opt? &open_lock : &new_passw;
}

void type_master_key() {
  char KINPUT[5] = {NULL};
  lcd.print("UNLOCK: ");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    char k = kpad.waitForKey();
    KINPUT[i] = k;
    lcd.print("*");
  }
  KINPUT[4] = '\0';

  if (strcmp(KINPUT, masterKey) != 0) {
    incorrect_password();
    return;
  }

  screen = &master_menu;
}

void new_passw() {
  char KINPUT[5] = {NULL};
  lcd.print("NEW PASSWORD: ");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    char k = kpad.waitForKey();
    KINPUT[i] = k;
    lcd.print("*");
  }
  KINPUT[4] = '\0';
  set_passw(KINPUT);
  screen = &master_menu;
}

void open_lock() {
  char PASSW[5] = { '\0' };
  char KINPUT[5] = { '\0' };
  bool has_passw = get_passw(PASSW);
  if (!has_passw) {
    lcd.print(" V     NO     V ");
    lcd.setCursor(0, 1);
    lcd.print(" o  PASSWORD  o ");
    delay(2000);
    screen = &master_menu;
    return;
  }
  lcd.print("PASSWORD: ");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    char k = kpad.waitForKey();
    KINPUT[i] = k;
    lcd.print("*");
  }
  KINPUT[4] = '\0';
  if (strcmp(KINPUT, PASSW) != 0) {
    incorrect_password();
    return;
  }

  lcd.clear();
  lcd.home();
  lcd.print(openedLock? "CLOSING..." : "OPENING...");
  openedLock = !openedLock;
  PORTC = B00000001;
  tone(16, 2500, 1000);
  delay(2000);
  PORTC = B00000000;
  screen = &master_menu;
}
