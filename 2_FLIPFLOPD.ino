#define LED0 3
#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7

#define PINC 9
#define PINCLK 8

bool D0 = 0, D1 = 0;
volatile bool Q0 = 0, Q1 = 0;
bool C = 0;
bool S0 = 0, S1 = 0, S2 = 0, S3 = 0;

bool flip_flop_D(bool D) {
  return D;
}

void CLK() {
  Q0 = flip_flop_D(D0);
  Q1 = flip_flop_D(D1);
}

void clockGenerator(byte pino, float f) {
  static bool state = LOW;
  static unsigned long t1, t0;
  t1 = millis();
  if (t1 - t0 >= 500. / f) {
    digitalWrite(pino, state = !state);
    t0 = t1;
  }
}

void funcoes_estado_seguinte() {
  D1 = (!Q1 & !Q0 & C) | (!Q1 & Q0 & !C) | (Q1 & !Q0 & !C);
  D0 = (Q1 & !Q0 & !C) | (!Q1 & !Q0);
}

void funcoes_saida() {
  S0 = !Q0 & !Q1;
  S1 = Q0 & !Q1;
  S2 = !Q0 & Q1;
  S3 = Q0 & Q1;
}

void escreve_saidas() {
  digitalWrite(LED0, S0);
  digitalWrite(LED1, S0 | S1);
  digitalWrite(LED2, S1 | S2);
  digitalWrite(LED3, S2 | S3);
  digitalWrite(LED4, S3);
}

void setup() {
  pinMode(PINC, INPUT_PULLUP);
  pinMode(11, INPUT);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  attachInterrupt(0, CLK, RISING);
  interrupts();

}

void loop() {

  C = !digitalRead(PINC);
  funcoes_estado_seguinte();
  funcoes_saida();
  escreve_saidas();
  clockGenerator(PINCLK, 1);
}
