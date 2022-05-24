#include "simple-cc.h"

// global var
Token *token;
char *user_input; //入力されたプログラム
Node *code[100];
LVar *locals;
Inst *inst;
Label *label;

//============
// utility function
//============

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

bool matchstr(const char *p1, char *p2) {
  return strncmp(p1, p2, strlen(p1)) == 0;
}

bool matchstr_plus(const char *p1, char *p2) {
  int len = strlen(p1);
  return strncmp(p1, p2, len) == 0 && !isalnumu(p2[len]);
}

bool isalnumu(char c) {
  return isalnum(c) || c == '_';
}

//============

int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }

  user_input = argv[1];
  //tokenize
  token = tokenize();
  //parse
  program();

  //initial code
  printf("LI 6,1\n");
  printf("SLL 6,12\n"); // rsp = 4096
  printf("PUSH 5\n");
  printf("MOV 5,6\n");

  // generate code
  for (int i = 0; code[i] != NULL; i++) {
    gen(code[i]);
    printf("POP 0\n");
  }

  //final code
  printf("MOV 6,5\n");
  printf("POP 0\n");
  printf("HLT\n");

  return 0;
}
