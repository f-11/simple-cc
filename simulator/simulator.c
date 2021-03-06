#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_PC 4096

typedef struct Inst Inst;
// instructions
typedef enum {
  ADD, SUB, AND, OR, XOR, CMP,
  MOV,
  SLL, SLR, SRL, SRA,
  IN, OUT,
  HLT,
  LD, ST,
  LI, ADDI, CMPI,
  B, BE, BLT, BLE, BNE, BR, BAL
} InstKind;

struct Inst {
  InstKind kind;
  int r0;     // Rd, Rs (OUT), Ra (LD,ST), Rb (LI), B* (d)
  int r1;     // Rs, d (shift*,LD,ST,ADDI,CMPI)
  int r2;     // Rb (LD,ST)
  int line;
};

Inst inst[512];
int mem[4096];
int reg[8];
int pc;
int stack[100];
char szcv[4];
bool flag_debug = false;
int board[8];

FILE *fp;
int stack_adr = 0;
void stack_push(int x) {
  stack[stack_adr++] = x;
}
int stack_pop() {
  stack_adr--;
  return stack[stack_adr];
}

void set_option(char *str) {
  if (str[1] == 'd')
    flag_debug = true;
}

void update_board(int val, int d) {
  board[d] = val;
}

void show_board() {
  for (int i=0; i<8; i++) {
    int bd = board[i];
    for (int d = 0; d < 16; d++)
      if (((bd >> (15-d)) & 1) == 1)
        printf("@");
      else
        printf(".");
    printf("\n");
  }
}

void show_reg() {
  for (int i = 0; i < 8; i++)
    printf("    r[%d] = %d;\n", i, reg[i]);
}

void show_mem() {
  int i;
  for (i = 4095; i > 4089; i--)
    printf("    *(%d) = %d\n", i, mem[i]);
  for (i = 3; i >= 0; i--)
    printf("    *(%d) = %d\n", i, mem[i]);
}

void exec(Inst inst) {
  int r0 = inst.r0, r1 = inst.r1, r2 = inst.r2;
  int tmp;
  switch (inst.kind) {
  case ADD:
    reg[r0] = reg[r0] + reg[r1];
    break;
  case SUB:
    reg[r0] = reg[r0] - reg[r1];
    break;
  case AND:
    reg[r0] = reg[r0] & reg[r1];
    break;
  case OR:
    reg[r0] = reg[r0] | reg[r1];
    break;
  case XOR:
    reg[r0] = reg[r0] ^ reg[r1];
    break;
  case CMP:
    tmp = reg[r0] - reg[r1];
    if (tmp == 0) szcv[1] = 1;
    else  szcv[1] = 0;
    if (tmp < 0) szcv[0] = 1;
    else szcv[0] = 1;
    break;
  case MOV:
    reg[r0] = reg[r1];
    break;
  case SLL:
    reg[r0] = reg[r0] << r1;
    break;
  case SLR:
    reg[r0] = reg[r0] << r1;
    break;
  case SRL:
    reg[r0] = reg[r0] >> r1;
    break;
  case SRA:
    reg[r0] = reg[r0] >> r1;
    break;
  case IN:
    // nop
    break;
  case OUT:
    printf("OUT: %d\n", reg[r0]); 
    update_board(reg[r0], r1);
    show_board();
    break;
  case HLT:
    pc = MAX_PC;
    break;
  case LD:
    if (reg[r2] + r1 < 0 || 4095 < reg[r2] + r1) {
      printf("error at line %d.\n", inst.line);
      printf("LD: r[%d] = mem[%d+%d]\n", r0, reg[r2], r1); 
      exit(1);
    }
    reg[r0] = mem[reg[r2] + r1];
    break;
  case ST:
    if (reg[r2] + r1 < 0 || 4095 < reg[r2] + r1) {
      printf("error at line %d.\n", inst.line);
      printf("ST: mem[%d+%d] = %d\n", reg[r2], r1, reg[r0]); 
      exit(1);
    }
    mem[reg[r2] + r1] = reg[r0];
    break;
  case LI:
    reg[r0] = r1;
    break;
  case ADDI:
    reg[r0] = reg[r0] + r1;
    break;
  case CMPI:
    tmp = reg[r0] - r1;
    if (tmp == 0) szcv[1] = 1;
    else  szcv[1] = 0;
    if (tmp < 0) szcv[0] = 1;
    else szcv[0] = 1;
    break;
  case B:
    pc += r0;
    break;
  case BE:
    if (szcv[1])
      pc += r0;
    break;
  case BLT:
    if (szcv[0])
      pc += r0;
    break;
  case BLE:
    if (szcv[0] || szcv[1])
      pc += r0;
    break;
  case BNE:
    if (!szcv[1])
      pc += r0;
    break;
  case BR:
    pc = stack_pop();
    break;
  case BAL:
    stack_push(pc);
    pc += r0;
    break;
  }
  pc++;
}

bool isalnumu(char c) {
  return isalnum(c) || c == '_';
}
bool matchstr_plus(const char *p1, char *p2) {
  int len = strlen(p1);
  return strncmp(p1, p2, len) == 0 && !isalnumu(p2[len]);
}

void hikisuu3(char *str, Inst *inst) {
  sscanf(str, "%d,%d,%d", &(inst->r0), &(inst->r1), &(inst->r2));
  return;
}
void hikisuu2(char *str, Inst *inst) {
  sscanf(str, "%d,%d", &(inst->r0), &(inst->r1));
  return;
}
void hikisuu1(char *str, Inst *inst) {
  sscanf(str, "%d", &(inst->r0));
  return;
}

Inst get_inst(char* str) {
  Inst inst;
  if (matchstr_plus("LD", str)) {
    inst.kind = LD;
    hikisuu3(str+3, &inst);
  } else if (matchstr_plus("ST", str)) {
    inst.kind = ST;
    hikisuu3(str+3, &inst);
  } else if (matchstr_plus("ADD", str)) {
    inst.kind = ADD;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("SUB", str)) {
    inst.kind = SUB;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("AND", str)) {
    inst.kind = AND;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("OR", str)) {
    inst.kind = OR;
    hikisuu2(str+3, &inst);
  } else if (matchstr_plus("XOR", str)) {
    inst.kind = XOR;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("CMP", str)) {
    inst.kind = CMP;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("MOV", str)) {
    inst.kind = MOV;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("SLL", str)) {
    inst.kind = SLL;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("SLR", str)) {
    inst.kind = SLR;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("SRL", str)) {
    inst.kind = SRL;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("SRA", str)) {
    inst.kind = SRA;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("IN", str)) {
    inst.kind = IN;
    hikisuu2(str+3, &inst);
  } else if (matchstr_plus("LI", str)) {
    inst.kind = LI;
    hikisuu2(str+3, &inst);
  } else if (matchstr_plus("ADDI", str)) {
    inst.kind = ADDI;
    hikisuu2(str+5, &inst);
  } else if (matchstr_plus("CMPI", str)) {
    inst.kind = CMPI;
    hikisuu2(str+5, &inst);
  } else if (matchstr_plus("OUT", str)) {
    inst.kind = OUT;
    hikisuu2(str+4, &inst);
  } else if (matchstr_plus("B", str)) {
    inst.kind = B;
    hikisuu1(str+2, &inst);
  } else if (matchstr_plus("BE", str)) {
    inst.kind = BE;
    hikisuu1(str+3, &inst);
  } else if (matchstr_plus("BLT", str)) {
    inst.kind = BLT;
    hikisuu1(str+4, &inst);
  } else if (matchstr_plus("BLE", str)) {
    inst.kind = BLE;
    hikisuu1(str+4, &inst);
  } else if (matchstr_plus("BNE", str)) {
    inst.kind = BNE;
    hikisuu1(str+4, &inst);
  } else if (matchstr_plus("BAL", str)) {
    inst.kind = BAL;
    hikisuu1(str+4, &inst);
  } else if (matchstr_plus("BR", str)) {
    inst.kind = BR;
  } else if (matchstr_plus("HLT", str)) {
    inst.kind = HLT;
  } else {
    printf("Unexpected instruction: %s\n", str);
    exit(1);
  }
  return inst;
}

int main(int argc, char **argv) {
  int i = 0;
  char str[16];

  if (argc > 1) {
    if (argv[1][0] == '-') {
      set_option(argv[1]);
      fp = stdin;
    } else
      fp = fopen(argv[1], "r");
  } else {
    fp = stdin;
  }
  if (!fp) {
    printf("Cannot open file.\n");
    return 1;
  }
  
  while(fgets(str, 15, fp) != NULL) {
    inst[i] = get_inst(str);
    inst[i].line = i;
    i++;
  }

  while(pc < i)
    exec(inst[pc]);

  if(flag_debug) {
    show_reg();
    show_mem();
  }
  return reg[0];
}
