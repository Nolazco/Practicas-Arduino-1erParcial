#include <DHT.h>

const char DIGITOS[] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101,
  0b00000111, 0b01111111, 0b01101111, 0b01100011, 0b00111001, 0b01110001, 0b00010010
};
int currentDisplay;
int mode;
bool lastState;

DHT dht(7, DHT11);

void setup() {
  currentDisplay = 0;
  mode = 0;
  lastState = LOW;
  DDRD = 0b01111111;
  DDRB = 0b00001111;
  dht.begin();
}

void loop() {
  stateChange();
  if (mode < 2) 
    displayTemperature(mode == 1);
  else
    displayHumidity();
  multiplex();
}

void stateChange() {
  bool currentState = digitalRead(12);
  if (currentState > lastState)
    mode = mode > 1? 0 : mode + 1;
  lastState = currentState;
}

void multiplex() {
  PORTB = 0b00001111;
  switch(currentDisplay) {
    case 0: PORTB = 0b00001110; break;
    case 1: PORTB = 0b00001101; break;
    case 2: PORTB = 0b00001011; break;
    case 3: PORTB = 0b00000111; break;
  }

  currentDisplay++;
  if (currentDisplay > 3)
    currentDisplay = 0;
}

void displayTemperature(bool isFarenheint) {
  float temperature = dht.readTemperature(isFarenheint);
  switch (currentDisplay) {
    case 0: PORTD = isFarenheint? DIGITOS[12] : DIGITOS[11]; break;
    case 1: PORTD = DIGITOS[10]; break;
    case 2: PORTD = isnan(temperature)? 0b01000000 : DIGITOS[(int)temperature % 10]; break;
    case 3: PORTD = isnan(temperature)? 0b01000000 : DIGITOS[(int)(temperature / 10) % 10]; break;
  }
}

void displayHumidity() {
  float humidity = dht.readHumidity();
  switch (currentDisplay) {
    case 0: PORTD = DIGITOS[13]; break;
    case 1: PORTD = isnan(humidity)? 0b01000000 : DIGITOS[(int)humidity % 10]; break;
    case 2: PORTD = isnan(humidity)? 0b01000000 : DIGITOS[(int)(humidity / 10) % 10]; break;
    case 3: PORTD = isnan(humidity)? 0b01000000 : DIGITOS[(int)(humidity / 100) % 10]; break;
  }
}
