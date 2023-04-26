#include <LiquidCrystal.h>
#define RS 0
#define EN 1
#define D4 2
#define D5 3
#define D6 4
#define D7 5

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
bool inMenu = true;
int mode = 0;
int lastTimestamp = 0;

struct pushButton {
  int pin;
  bool lastState;
  bool currentState;
  void (*onPush)();
} btnSelect, btnMenu;

byte selectArrow[] = {
  B00000, B00100, B11110, B11111,
  B11110, B00100, B00000, B00000
};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, selectArrow);
  btnSelect = {12, true, true, &onSelect};
  btnMenu = {11, true, true, &onMenu};
  lastTimestamp = 0;
  inMenu = true;
  mode = 0;
  DDRB = B00000111;
  pinMode(btnMenu.pin, INPUT_PULLUP);
  pinMode(btnMenu.pin, INPUT_PULLUP);
  PORTB = B00000000;
}

void loop() {
  buttonsProcess();
  if ((millis() - lastTimestamp) > 333) {
    lcd.home();
    lcd.clear();
    if (!inMenu) {
      switch(mode) {
        case 0:
          initialize();
          break;
        case 1:
          calibrate();
          break;
        case 2:
          mesure();
          break;
      }
    } else {
      PORTB = B00000000;
      displayMenu();
    }
    lastTimestamp = millis();
  }
}

void initialize() {
  digitalWrite(8, HIGH);
  lcd.print("Inicializando...");
}
void calibrate() {
  digitalWrite(9, HIGH);
  lcd.print("Calibrando...");
}
void mesure() {
  digitalWrite(10, HIGH);
  lcd.print("Midiendo...");
}

void onSelect() {
  inMenu = !inMenu;
}

void onMenu() {
  if (!inMenu) return;
  mode = mode > 1? 0 : mode + 1;
}

void displayMenu() {
  lcd.setCursor(0, 0);
  lcd.print("===== MENU =====");
  lcd.setCursor(0, 1);
  lcd.print(" INI  CAL  MED");
  switch(mode) {
    case 0:
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      break;
    case 1:
      lcd.setCursor(5, 1);
      lcd.write(byte(0));
      break;
    case 2:
      lcd.setCursor(10, 1);
      lcd.write(byte(0));
      break;
  }
}

void buttonsProcess() {
  struct pushButton *buttons[2] = {&btnSelect, &btnMenu};
  for(int i = 0; i < 2; i++) {
    struct pushButton *currentBtn = buttons[i];
    currentBtn->currentState = digitalRead(currentBtn->pin);
    if (currentBtn->currentState < currentBtn->lastState)
      currentBtn->onPush();
    currentBtn->lastState = currentBtn->currentState;
  }
}
