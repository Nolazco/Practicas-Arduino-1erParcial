#define MAX_CYCLES 4000

const int digits[] = { 
  0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 
  0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111 
};

unsigned int cDisplay, counter, ciclo;
int pushed = 0;

void setup() {
  DDRD = 0b01111111;
  DDRB = 0b00000111;
  cDisplay = 0;
  counter = 0;
  ciclo = 0;
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  PORTB = 0b00000111;
  pushed = digitalRead(12);
  if (pushed)
    contar();
  showNumber();
  multiplex();
}

void multiplex(){
  switch(cDisplay){
     case 0: PORTB = 0b00000110; break;
     case 1: PORTB = 0b00000101; break;
     case 2: PORTB = 0b00000011; break;
  }
  cDisplay++;
  if(cDisplay > 2)
    cDisplay = 0;
}

void showNumber(){
  switch(cDisplay){
    case 0: PORTD = digits[counter % 10]; break;
    case 1: PORTD = digits[(counter / 10) % 10]; break;
    case 2: PORTD = digits[(counter / 100) % 10]; break;
  }
}

void contar(){
  ciclo++;
  if(ciclo > MAX_CYCLES){
    counter++;
    if(counter > 999)
      counter = 0;
    ciclo = 0;
  }
}
