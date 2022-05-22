#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>


typedef struct Token Token;
typedef struct Node Node;
typedef struct LVar LVar;
typedef struct Inst Inst;
typedef struct Label Label;

// Token
typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str; // token string
  int len;  // length of str
};

// AST
typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_EQL, // ==
  ND_NEQL,// !=
  ND_LT,  // <
  ND_LTE, // <=
  ND_ASSIGN, // =
  ND_LVAR,   // local variables
  ND_NUM,
} NodeKind;

struct Node {
  NodeKind kind;
  Node *lhs;  //左辺
  Node *rhs;  //右辺
  int val;  //ND_NUMのとき
  int offset;   //ND_LVARのとき変数の位置を示す
};

// local variables
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

// instructions
typedef enum {
  ADD,
  SUB,
  AND,
  OR,
  XOR,
  CMP,
  MOV,
  SLL,
  SLR,
  SRL,
  SRA,
  IN,
  OUT,
  HLT,
  LD,
  LI,
  ADDI,
  CMPI,
  B,
  BE,
  BLT,
  BLE,
  BNE,
  BR,
  BAL
} InstKind;

struct Inst {
  InstKind kind;
  int r0, r1, r2;
  int linenum; // アセンブリにおける行番号
  Inst *next;
};

struct Label {
  char *name;
  int len;
  Inst *inst;
  Label *next;
};

//==declare==
extern Token *token;
extern char *user_input; //入力されたプログラム
extern Node *code[100];
extern LVar *locals;
extern Inst *inst;
extern Label *label;

//==functions==

//main.c
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool matchstr(const char *p1, char *p2);

//tokenize.c
bool consume(const char *op);
Token *consume_ident();
void expect(const char *op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize ();

//parse.c
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void program();

//codegen.c
void gen(Node *node);

//inst.c
void add_inst(InstKind kind, int r0, int r1, int r2);
void add_call(char *name);
void add_label(char *name);
