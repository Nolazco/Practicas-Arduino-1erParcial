const int DIGITS[] = { 
  0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 
  0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111 
};

int cont;

void setup() {
  DDRD = 0b11111111;
  cont = 0;
}

void loop() {
  PORTD = DIGITS[cont];
  cont++;
  if(cont > 9)
    cont = 0;
  delay(1000);
}
