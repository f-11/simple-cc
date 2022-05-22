#include "simple-cc.h"


void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " "); //位置揃え
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待する記号(op)のときはトークンを一つ進める
// 真偽を返す
bool consume(const char *op) {
  if (token->kind != TK_RESERVED || 
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

// 次のトークンがローカル変数ならその変数のベースポインタからのオフセットを返す
int consume_ident_and_return_offset() {
  if (token->kind != TK_IDENT)
    return 0;
  int ofs = token->str[0] - 'a' + 1;
  token = token->next;
  return ofs;
}

// 次のトークンが期待する記号(op)のときはトークンを一つ進める
// 偽ならエラー
void expect(const char *op) {
  if (token->kind != TK_RESERVED ||
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    error("Expecting %s, but not.", op);
  token = token->next;
}

// 次のトークンが数値のときはトークンを一つ進める
// 偽ならエラー
int expect_number() {
  if (token->kind != TK_NUM)
    error("not a number.");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

bool matchstr(const char *p1, char *p2) {
  return strncmp(p1, p2, strlen(p1)) == 0;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

Token *tokenize () {
  char *p = user_input;
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }
    if (matchstr("==", p) || matchstr("!=", p) ||
        matchstr("<=", p) || matchstr(">=", p) ) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p += 2;
      continue;
    }
    if (strchr("+-*/()><=;", *p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }
    if ('a' <= *p && *p <= 'z') {
      cur = new_token(TK_IDENT, cur, p++, 1);
      continue;
    }
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 0);
      char *q = p;
      cur->val = strtol(p, &p, 10);
      cur->len = p - q;
      continue;
    }

    error("Tokenize failed.");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

// parser

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

Node *new_node_ident(int offset) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;
  node->offset = offset;
  return node;
}

// program = stmt*
// stmt = expr ";"
// expr = assign
// assign = equality ("=" assign)?
// equality = relational ("==" relational | "!=" relational)*
// relational = add ("<=" add | "<" add | ">=" add | ">" add)*
// add = mul ("+" mul | "-" mul)*
// mul = unary ("*" unary | "/" unary)*
// unary = ("+" | "-")? primay
// primary = num | ident | "(" expr ")"

Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

void program() {
  int i = 0;
  while (!at_eof())
    code[i++] = stmt();
  code[i] = NULL;
}

Node *stmt() {
  Node *node = expr();
  expect(";");
  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();
  if (consume("="))
    return new_node(ND_ASSIGN, node, assign());
  else
    return node;
}

Node *equality() {
  Node *node = relational();

  for(;;) {
    if (consume("=="))
      node = new_node(ND_EQL, node, relational());
    else if (consume("!="))
      node = new_node(ND_NEQL, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<="))
      node = new_node(ND_LTE, node, add());
    else if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume(">="))
      node = new_node(ND_LTE, add(), node);
    else if (consume(">"))
      node = new_node(ND_LT, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-")) 
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  int ofs = consume_ident_and_return_offset();
  if (ofs)
    return new_node_ident(ofs);

  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  } 

  return new_node_num(expect_number());
}
