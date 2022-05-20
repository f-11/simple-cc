#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

Token *token;

char *user_input; //入力されたプログラム

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
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// 次のトークンが期待する記号(op)のときはトークンを一つ進める
// 偽ならエラー
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("not %c", op);
  token = token->next;
}

// 次のトークンが数値のときはトークンを一つ進める
// 偽ならエラー
int expect_number() {
  if (token->kind != TK_NUM)
    error("数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

Token *tokenize (char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("Tokenize failed.");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }

  token = tokenize(argv[1]);

  printf("LI 0,%d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("LI 1,%d\n", expect_number());
      printf("ADD 0 1\n");
      continue;
    }

    expect('-');
    printf("LI 1,%d\n", expect_number());
    printf("SUB 0,1\n");
  }
  printf("HLT\n");

  return 0;
}
