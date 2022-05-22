#include "simple-cc.h"

void add_inst(InstKind kind, int r0, int r1, int r2){
  Inst *new_inst = calloc(1, sizeof(Inst));
  new_inst->kind = kind;
  new_inst->r0 = r0;
  new_inst->r1 = r1;
  new_inst->r2 = r2;

  inst->next = new_inst;
  inst = new_inst;
}

void add_call(char *name) {
  Inst *new_inst = calloc(1, sizeof(Inst));
  new_inst->kind = BAL;

  inst->next = new_inst;
  inst = new_inst;
}

void add_label(char *name){
  Label *new_label = calloc(1, sizeof(Label));
  new_label->name = name;

  label->next = new_label;
  label = new_label;
}
