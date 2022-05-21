#include "simple-cc.h"

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("LI 0,%d\n", node->val);
    printf("PUSH 0\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("POP 0\n");
  printf("POP 1\n");

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
  }

  printf("PUSH 0\n");
}

