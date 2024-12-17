#include "shared.h"


void printStdOut(Computer *C) {
  if (!C->std_out.length) return;

  int i;
  for (i = 0; i < C->std_out.length - 1; i++) {
    printf("%d,", C->std_out.x[i]);
  }

  printf("%d\n", C->std_out.x[i]);
}


void printComputer(Computer *C) {
  printf("Register A: %lld\n", C->A);
  printf("Register B: %lld\n", C->B);
  printf("Register C: %lld\n", C->C);
  printf("\nProgram:");
  printArray(&C->program);
}


void freeComputer(Computer *C) {
  freeArray(&C->program);
  freeArray(&C->std_out);
}


Computer copyComputer(Computer *C) {
  Computer V;
  V.A = C->A;
  V.B = C->B;
  V.C = C->C;

  V.ip = C->ip;

  V.program = copyArray(&C->program);
  V.std_out = copyArray(&C->std_out);

  return V;
}


Computer readfile(const char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Unable to open file!\n");
    exit(-1);
  }

  Computer C;
  C.program = initArray(INITIAL_CAPACITY);
  C.std_out = initArray(INITIAL_CAPACITY);
  C.ip = 0;

  char c, mode = '0', reg = '0';
  int num = 0, onum = 0;
  while ((c = getc(file)) != EOF) {

    switch (mode) {
      case '0': switch (c) {
          case 'P': {mode = 'P'; break;};
          case 'R': {mode = 'R'; break;};
      }

      case 'R': {
        if (isdigit(c)) {
          num = 10 * num + c - '0';
          onum = 1;

        } else if (!onum && reg == '0'){
          if (c >= 'A' && c <= 'C') reg = c;

        } else if (onum) {
          switch (reg) {
            case 'A': C.A = num;
            case 'B': C.B = num;
            case 'C': C.C = num;
          }

          mode = '0';
          reg  = '0';
          num  =  0;
          onum =  0;
        }
        break;
      }

      case 'P': {
        if (isdigit(c)) {
          num  = 10 * num + c - '0';
          onum = 1;

        } else {
          if (onum) append(&C.program, num); 
          num = 0;
          onum = 0;
        }
        break;
      }
    }

    if (c == '\n') {
      mode = '0';
      reg  = '0';
      num  =  0;
      onum =  0;
      continue;
    }
  }


  return C;
}


long long int evalComboOperand(int operand, Computer *C) {
  if (operand >= 0 && operand <= 3) {
    return operand;
  }

  switch (operand) {
    case 4: return C->A;
    case 5: return C->B;
    case 6: return C->C;
    case 7: {printf("Invalid operand 7!\n"); exit(-1);}
    default: return operand;
  }
}


int evalLiteralOperand(int operand) {
  return operand;
}


void opcodeADV(int operand, Computer *C) {
  long long int numerator = C->A;
  long long int denominator = pow(2, evalComboOperand(operand, C));
  C->A = numerator / denominator;
}


void opcodeBXL(int operand, Computer *C) {
  C->B = evalLiteralOperand(operand) ^ C->B;
}


void opcodeBST(int operand, Computer *C) {
  C->B = evalComboOperand(operand, C) % 8;
}


void opcodeJNZ(int operand, Computer *C) {
  if (!C->A) return;
  C->ip = operand;
}


void opcodeBXC(int operand, Computer *C) {
  C->B = C->B ^ C->C;
}


void opcodeOUT(int operand, Computer *C) {
  append(&C->std_out, evalComboOperand(operand, C) % 8);
}


void opcodeBDV(int operand, Computer *C) {
  long long int numerator = C->A;
  long long int denominator = pow(2, evalComboOperand(operand, C));
  C->B = numerator / denominator;
}


void opcodeCDV(int operand, Computer *C) {
  long long int numerator = C->A;
  long long int denominator = pow(2, evalComboOperand(operand, C));
  C->C = numerator / denominator;
}


int evalOpcode(Computer *C) {
  if (C->ip >= C->program.length) {
    // if returns 0, the program halts, else it continues
    return 0;
  }

  int opcode  = C->program.x[C->ip++];
  int operand = C->program.x[C->ip++];

  void (*funcs[8])(int, Computer*) = {
    opcodeADV,
    opcodeBXL,
    opcodeBST,
    opcodeJNZ,
    opcodeBXC,
    opcodeOUT,
    opcodeBDV,
    opcodeCDV
  }; 

  funcs[opcode](operand, C);
  return 1; 
}


void evalProgram(Computer *C) {
  int running = 1;
  while (running) running = evalOpcode(C);
}
