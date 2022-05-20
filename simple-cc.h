#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>


typedef struct Token Token;
typedef struct Node Node;

// Token
typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

// AST
typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
} NodeKind;

struct Node {
  NodeKind kind;
  Node *lhs;  //左辺
  Node *rhs;  //右辺
  int val;  //ND_NUMのとき
};

//==declare==
extern Token *token;
extern char *user_input; //入力されたプログラム

//==functions==

//parse.c
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char op);
void expect(char op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind, Token *cur, char *str);
Token *tokenize (char *p);

//codegen.c
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *mul();
Node *primary();
void gen(Node *node);
