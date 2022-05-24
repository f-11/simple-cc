#include "simple-cc.h"

void add_inst(InstKind kind, ...){
  va_list ap;
  va_start(ap, kind);

  Inst *new_inst = calloc(1, sizeof(Inst));
  new_inst->kind = kind;
  switch (kind) {
  case LD:    // 引数3
  case ST:
    new_inst->r0 = va_arg(ap, int);
    new_inst->r1 = va_arg(ap, int);
    new_inst->r2 = va_arg(ap, int);
    break;
  case ADD:   // 引数2
  case SUB:
  case AND:
  case OR:
  case XOR:
  case CMP:
  case MOV:
  case SLL:
  case SLR:
  case SRL:
  case SRA:
  case IN:
  case LI:
  case ADDI:
  case CMPI:
    new_inst->r0 = va_arg(ap, int);
    new_inst->r1 = va_arg(ap, int);
    break;
  case OUT:   // 引数1
  case B:
  case BE:
  case BLT:
  case BLE:
  case BNE:
  case BAL:
    new_inst->r0 = va_arg(ap, int);
    break;
  default:
    break;
  }
  new_inst->linenum = inst->linenum + 1;

  inst->next = new_inst;
  inst = new_inst;

  va_end(ap);
}

void add_call(char *name) {
  Inst *new_inst = calloc(1, sizeof(Inst));
  new_inst->kind = BAL;
  new_inst->name = name;

  inst->next = new_inst;
  inst = new_inst;
}
void add_jump(InstKind kind, char *name) {
  Inst *new_inst = calloc(1, sizeof(Inst));
  new_inst->kind = kind;
  new_inst->name = name;

  inst->next = new_inst;
  inst = new_inst;
}

void add_label(char *name){
  Label *new_label = calloc(1, sizeof(Label));
  new_label->name = name;
  new_label->inst = inst; //ジャンプ先の一行前の命令

  label->next = new_label;
  label = new_label;
}

void add_pop(int n) {
  add_inst(LD, n, 0, 6);
  add_inst(ADDI, 6, 1);
}

void add_push(int n) {
  add_inst(ADDI, 6, -1);
  add_inst(ST, n, 0, 6);
}

int search_label_and_return_line(char *name, Label *head) {
  for (Label *l = head; l; l = l->next)
    if (!strcmp(name, l->name))
      return l->inst->linenum;
  error("No such label: %s\n", name);
  return 0;
}

void link(Inst *ins, Label *lab) {
  if (ins == NULL)
    return;
  switch (ins->kind) {
  case B:
  case BE:
  case BLT:
  case BLE:
  case BNE:
  case BAL:
    if (ins->name) {
      int adr = search_label_and_return_line(ins->name, lab);
      ins->r0 = adr - (ins->linenum);
    }
    break;
  default:
    ;
  }
  link(ins->next, lab);
}

void print_inst(Inst *head) {
  switch (head->kind) {
  case ADD:
    printf("ADD %d,%d\n", head->r0, head->r1);
    break;
  case SUB:
    printf("SUB %d,%d\n", head->r0, head->r1);
    break;
  case AND:
    printf("SUB %d,%d\n", head->r0, head->r1);
    break;
  case OR:
    printf("OR %d,%d\n", head->r0, head->r1);
    break;
  case XOR:
    printf("XOR %d,%d\n", head->r0, head->r1);
    break;
  case CMP:
    printf("CMP %d,%d\n", head->r0, head->r1);
    break;
  case MOV:
    printf("MOV %d,%d\n", head->r0, head->r1);
    break;
  case SLL:
    printf("SLL %d,%d\n", head->r0, head->r1);
    break;
  case SLR:
    printf("SLR %d,%d\n", head->r0, head->r1);
    break;
  case SRL:
    printf("SRL %d,%d\n", head->r0, head->r1);
    break;
  case SRA:
    printf("SRA %d,%d\n", head->r0, head->r1);
    break;
  case IN:
    printf("IN %d,%d\n", head->r0, head->r1);
    break;
  case OUT:
    printf("OUT %d\n", head->r0); 
    break;
  case HLT:
    printf("HLT\n"); 
    break;
  case LD:
    printf("LD %d,%d,%d\n", head->r0, head->r1, head->r2);
    break;
  case ST:
    printf("ST %d,%d,%d\n", head->r0, head->r1, head->r2);
    break;
  case LI:
    printf("LI %d,%d\n", head->r0, head->r1);
    break;
  case ADDI:
    printf("ADDI %d,%d\n", head->r0, head->r1);
    break;
  case CMPI:
    printf("CMPI %d,%d\n", head->r0, head->r1);
    break;
  case B:
    printf("B %d\n", head->r0); 
    break;
  case BE:
    printf("BE %d\n", head->r0); 
    break;
  case BLT:
    printf("BLT %d\n", head->r0); 
    break;
  case BLE:
    printf("BLE %d\n", head->r0); 
    break;
  case BNE:
    printf("BNE %d\n", head->r0); 
    break;
  case BR:
    printf("BR\n"); 
    break;
  case BAL:
    printf("BAL %d\n", head->r0); 
    break;
  }
  if (head->next)
    print_inst(head->next);
}
