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
    case ND_EQL:
      printf("CMP 0,1\n");
      break;
    case ND_NEQL:
      printf("CMP 0,1\n");
      break;
    case ND_LT:
      printf("CMP 0,1\n");
      break;
    case ND_LTE:
      printf("CMP 0,1\n");
      break;
    default:
      ;
  }

  printf("PUSH 0\n");
}

