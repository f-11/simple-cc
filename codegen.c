#include "simple-cc.h"

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("expecting variable.\n");

  add_inst(MOV, 0, 5); // mov r0,rbp
  add_inst(ADDI, 0, -(node->offset)); // set address to r0
  add_push(0);
}

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
    add_inst(LI, 0, node->val);
    add_push(0);
    return;
  case ND_LVAR:
    gen_lval(node);
    add_pop(0);
    add_inst(LD, 0,0,0);
    add_push(0);
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);
    add_pop(1);
    add_pop(0);
    add_inst(ST, 1,0,0);
    add_push(1);
    return;
  case ND_RETURN:
    gen(node->lhs);
    add_pop(0);
    add_inst(BR);
    return;
  case ND_IF:       
    gen(node->lhs);           //条件評価 A
    add_pop(0);
    add_inst(CMPI, 0,1);
    char *elselabel = get_unique_str("else");
    char *ifendlabel = get_unique_str("ifend");
    if (node->rhs) {          // "if" "(" A ")" B "else" C 
      add_jump(BE, elselabel);
      gen(node->stmt1);        // B
      add_jump(B, ifendlabel);
      add_label(elselabel);
      gen(node->stmt2);       // C
      add_label(ifendlabel);
    } else {                  // "if" "(" A ")" B
      add_jump(BE, ifendlabel);
      gen(node->stmt1);       // B
      add_label(ifendlabel);
    }
    return;
  default:
    ;
  }

  gen(node->lhs);
  gen(node->rhs);

  add_pop(1);
  add_pop(0);

  switch(node->kind) {
    case ND_ADD:
      add_inst(ADD, 0,1);
      break;
    case ND_SUB:
      add_inst(SUB, 0,1);
      break;
    case ND_MUL:
      //add_call("MUL");
      break;
    case ND_DIV:
      //add_call("DIV");
      break;
    case ND_EQL: // 条件が真なら0,偽なら1がスタックに
      add_inst(CMP, 0,1);
      add_inst(BE, 2);
      add_inst(LI, 0,1);
      add_inst(B, 1);
      add_inst(LI, 0,0);
      break;
    case ND_NEQL:
      add_inst(CMP, 0,1);
      add_inst(BNE, 2);
      add_inst(LI, 0,1);
      add_inst(B, 1);
      add_inst(LI, 0,0);
      break;
    case ND_LT:
      add_inst(CMP, 0,1);
      add_inst(BLT, 2);
      add_inst(LI, 0,1);
      add_inst(B, 1);
      add_inst(LI, 0,0);
      break;
    case ND_LTE:
      add_inst(CMP, 0,1);
      add_inst(BLE, 2);
      add_inst(LI, 0,1);
      add_inst(B, 1);
      add_inst(LI, 0,0);
      break;
    default:
      ;
  }

  add_push(0);
}

