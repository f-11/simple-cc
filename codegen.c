#include "simple-cc.h"

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("expecting variable.\n");

  printf("MOV 0,5\n"); // mov r0,rbp
  printf("SUBI 0,%d\n", node->offset); // set address to r0
  printf("PUSH 0\n");
}

void gen(Node *node) {
  switch (node->kind) {
  case ND_NUM:
    printf("LI 0,%d\n", node->val);
    printf("PUSH 0\n");
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("POP 0\n");
    printf("LD 0,0,0\n");
    printf("PUSH 0\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);
    printf("POP 1\n");
    printf("POP 0\n");
    printf("ST 1,0,0\n");
    printf("PUSH 1\n");
    return;
  case ND_RETURN:
    gen(node->lhs);
    printf("POP 0\n");
    printf("BR\n");
    return;
  case ND_IF:       
    gen(node->lhs);           //条件評価 A
    printf("POP 0\n");
    printf("CMPI 0,1\n");
    if (node->rhs) {          // "if" "(" A ")" B "else" C 
      printf("BE -> else\n");
      gen(node->stmt1);        // B
      printf("B -> end\n");
      printf("-> else\n");
      gen(node->stmt2);       // C
      printf("-> end\n");
    } else {                  // "if" "(" A ")" B
      printf("BE -> end\n");
      gen(node->stmt1);       // B
      printf("-> end\n");
    }
    return;
  default:
    ;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("POP 1\n");
  printf("POP 0\n");

  switch(node->kind) {
    case ND_ADD:
      printf("ADD 0,1\n");
      break;
    case ND_SUB:
      printf("SUB 0,1\n");
      break;
    case ND_MUL:
      printf("MUL 0,1\n");
      break;
    case ND_DIV:
      printf("DIV 0,1\n");
      break;
    case ND_EQL: // 条件が真なら0,偽なら1がスタックに
      printf("CMP 0,1\n");
      printf("BE 2\n");
      printf("LI 0,1\n");
      printf("B 1\n");
      printf("LI 0,0\n");
      break;
    case ND_NEQL:
      printf("CMP 0,1\n");
      printf("BNE 2\n");
      printf("LI 0,1\n");
      printf("B 1\n");
      printf("LI 0,0\n");
      break;
    case ND_LT:
      printf("CMP 0,1\n");
      printf("BLT 2\n");
      printf("LI 0,1\n");
      printf("B 1\n");
      printf("LI 0,0\n");
      break;
    case ND_LTE:
      printf("CMP 0,1\n");
      printf("BLE 2\n");
      printf("LI 0,1\n");
      printf("B 1\n");
      printf("LI 0,0\n");
      break;
    default:
      ;
  }

  printf("PUSH 0\n");
}

