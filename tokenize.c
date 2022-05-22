#include "simple-cc.h"

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

