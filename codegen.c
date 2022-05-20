#include "simple-cc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node(ND_ADD, node, mul());
    else if (consume('-')) 
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = primary();

  for (;;) {
    if (consume('*'))
      node = new_node(ND_MUL, node, primary());
    else if (consume('/'))
      node = new_node(ND_DIV, node, primary());
    else
      return node;
  }
}

Node *primary() {
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }

  return new_node_num(expect_number());
}

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
  }

  printf("PUSH 0\n");
}

