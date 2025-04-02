// INPUT PINS
#define PINA2 A0
#define PINA1 A1
#define PINA0 A2
#define PINB2 A3
#define PINB1 A6
#define PINB0 A7
#define PINENABLE 2
#define PINC0 4
#define PINC1 3

// OUTPUT PINS
#define PINR2 7
#define PINR1 6
#define PINR0 5
#define PINCYBWO 8
#define PINZ 9
#define PINOV 10

void setup() {

  pinMode(PINA2, INPUT_PULLUP);
  pinMode(PINA1, INPUT_PULLUP);
  pinMode(PINA0, INPUT_PULLUP);
  pinMode(PINB2, INPUT_PULLUP);
  pinMode(PINB1, INPUT_PULLUP);
  pinMode(PINB0, INPUT_PULLUP);
  pinMode(PINENABLE, INPUT_PULLUP);
  pinMode(PINC0, INPUT_PULLUP);
  pinMode(PINC1, INPUT_PULLUP);

  pinMode(PINR2, OUTPUT);
  pinMode(PINR1, OUTPUT);
  pinMode(PINR0, OUTPUT);
  pinMode(PINCYBWO, OUTPUT);
  pinMode(PINZ, OUTPUT);
  pinMode(PINOV, OUTPUT);

}

void loop() { 

  // X0, X1, X2
  bool VAL_A[3] = {readAnalogPin(PINA0), readAnalogPin(PINA1), readAnalogPin(PINA2)};
  bool VAL_B[3] = {readAnalogPin(PINB0), readAnalogPin(PINB1), readAnalogPin(PINB2)};
  bool VAL_C[2] = {!digitalRead(PINC0), !digitalRead(PINC1)};
  bool VAL_E = !digitalRead(PINENABLE);

  bool VAL_R[3];
  bool VAL_CYBWO, VAL_Z, VAL_OV;

  ALU(VAL_A, VAL_B, VAL_C, VAL_E, VAL_R, &VAL_CYBWO, &VAL_Z, &VAL_OV); 
  
}

void ALU(bool VAL_A[], bool VAL_B[], bool VAL_C[], bool VAL_E, 
         bool VAL_R[], bool *VAL_CYBWO, bool *VAL_Z, bool *VAL_OV) {
  
  bool SUM_S[3] = {0, 0, 0}, SUB_S[3], NOR_S[] = {0, 0, 0}, NAND_S[] = {0, 0, 0}; 
  bool CyOut, BwOut;

  threeBitAdder(VAL_A, VAL_B, SUM_S, &CyOut);
  threeBitSubtractor(VAL_A, VAL_B, SUB_S, &BwOut);
  NOR(VAL_A, VAL_B, NOR_S);
  NAND(VAL_A, VAL_B, NAND_S);

  VAL_R[0] = MUX_4X1(SUM_S[0], SUB_S[0], NOR_S[0], NAND_S[0], VAL_C[0], VAL_C[1]);
  VAL_R[1] = MUX_4X1(SUM_S[1], SUB_S[1], NOR_S[1], NAND_S[1], VAL_C[0], VAL_C[1]);
  VAL_R[2] = MUX_4X1(SUM_S[2], SUB_S[2], NOR_S[2], NAND_S[2], VAL_C[0], VAL_C[1]);

  flags(VAL_R, VAL_A[2], VAL_B[2], VAL_C[0], CyOut, BwOut, VAL_CYBWO, VAL_OV, VAL_Z);

  enableMUX(VAL_R, VAL_CYBWO, VAL_OV, VAL_Z, VAL_E);

  outputLEDS(VAL_R, *VAL_CYBWO, *VAL_OV, *VAL_Z);
}

void fullAdder(bool A, bool B, bool *S, bool *C) {
  *S = A ^ B ^ *C;
  *C = (A & B) | (A & *C) | (B & *C);
}

void threeBitAdder(bool A[], bool B[], bool S[], bool *CyOut) {

  bool carry = false;

  for(int i = 0; i < 3; i++) {
    fullAdder(A[i], B[i], &S[i], &carry);
  }

  *CyOut = carry;

}

void fullSubtractor(bool A, bool B, bool *S, bool *Bw) {
  bool Bwi = *Bw;
  *S = A ^ B ^ *Bw;
  *Bw = (!A & B) | (!A & Bwi) | (B & Bwi);
}

void threeBitSubtractor(bool A[], bool B[], bool S[], bool *BwOut) {
  bool borrow = false;

  for(int i = 0; i < 3; i++) {
    fullSubtractor(A[i], B[i], &S[i], &borrow);
  }

  *BwOut = borrow;
}

void NOR(bool A[], bool B[], bool S[]) {
  for(int i = 0; i < 3; i++) {
    S[i] = !(A[i] | B[i]);
  }
}

void NAND(bool A[], bool B[], bool S[]) {
  for(int i = 0; i < 3; i++) {
    S[i] = !(A[i] & B[i]);
  }
}

bool MUX_4X1(bool A, bool B, bool C, bool D, bool C0, bool C1) {
  return (A & !C0 & !C1) | (B & C0 & !C1) | (C & !C0 & C1) | (D & C0 & C1);
}

void flags(bool R[], bool VAL_A2, bool VAL_B2, bool VAL_C0, bool CyOut, bool BwOut, bool *CyBwo, bool *Ov, bool *Z) {
  *CyBwo = !VAL_C0 & CyOut | VAL_C0 & BwOut; 
  *Ov = (!VAL_A2 & !VAL_B2 & R[2]) | (VAL_A2 & VAL_B2 & !R[2]);
  *Z = !R[0] & !R[1] & !R[2];
}

bool readAnalogPin(byte pin) {
  return analogRead(pin) < 100;
}

void enableMUX(bool R[], bool *CyBwo, bool *Ov, bool *Z, bool E) {
  R[0] = R[0] & E;
  R[1] = R[1] & E;
  R[2] = R[2] & E;
  *CyBwo = *CyBwo & E;
  *Z = *Z & E;
  *Ov = *Ov & E;
}

void outputLEDS(bool R[], bool CyBwo, bool Ov, bool Z) {
  digitalWrite(PINR0, R[0]);
  digitalWrite(PINR1, R[1]);
  digitalWrite(PINR2, R[2]);
  digitalWrite(PINCYBWO, CyBwo);
  digitalWrite(PINOV, Ov);
  digitalWrite(PINZ, Z);
}