#include "simple-cc.h"

// r[5] = rbp
// r[6] = rsp
// r[7] = zero

void gen_lvar(Node *node) {
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
    gen_lvar(node);
    add_pop(0);
    add_inst(LD, 0,0,0);
    add_inst(ADDI, 6, -1);
    add_push(0);
    return;
  case ND_ASSIGN:
    gen_lvar(node->lhs);
    gen(node->rhs);
    add_pop(1);
    add_pop(0);
    add_inst(ST, 1,0,0);
    add_inst(ADDI, 6, -1);
    add_push(1);
    return;
  case ND_RETURN:
    gen(node->lhs);
    add_pop(0);
    add_inst(BR);
    return;
  case ND_IF: {       
    gen(node->lhs);           //条件評価 A
    add_pop(0);
    add_inst(CMPI, 0,0);
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
    return; }
  case ND_WHILE: {             // while "(" A ")" B
    char *whilelabel = get_unique_str("while");
    char *whendlabel = get_unique_str("whend");
    add_label(whilelabel);
    gen(node->lhs);           //条件評価 A
    add_pop(0);
    add_inst(CMPI, 0,0);
    add_jump(BE, whendlabel);
    gen(node->stmt1);         // B
    add_jump(B, whilelabel);
    add_label(whendlabel);
    return; }
  case ND_FOR: {              // for "(" A? ";" B? ";" C? ")" D
    char *forlabel = get_unique_str("for");
    char *forendlabel = get_unique_str("forend");
    if (node->lhs)
      gen(node->lhs);           // A
    add_label(forlabel);
    gen(node->rhs);           // B
    add_pop(0);
    add_inst(CMPI, 0,0);
    add_jump(BE, forendlabel);
    gen(node->stmt2);         // D
    gen(node->stmt1);         // C
    add_jump(B, forlabel);
    add_label(forendlabel);
    return; }
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
      add_inst(MOV, 2, 0);
      add_inst(ADD, 0, 2);
      add_inst(ADDI, 1, -1);
      add_inst(CMPI, 1, 1);
      add_inst(BNE, -4);
      break;
    case ND_DIV:
      //add_call("DIV");
      break;
    case ND_EQL: // 条件が真なら1,偽なら0がスタックに
      add_inst(CMP, 0,1);
      add_inst(BE, 2);
      add_inst(LI, 0,0);
      add_inst(B, 1);
      add_inst(LI, 0,1);
      break;
    case ND_NEQL:
      add_inst(CMP, 0,1);
      add_inst(BNE, 2);
      add_inst(LI, 0,0);
      add_inst(B, 1);
      add_inst(LI, 0,1);
      break;
    case ND_LT:
      add_inst(CMP, 0,1);
      add_inst(BLT, 2);
      add_inst(LI, 0,0);
      add_inst(B, 1);
      add_inst(LI, 0,1);
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

