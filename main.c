#include "simple-cc.h"

// global var
Token *token;
char *user_input; //入力されたプログラム
Node *code[100];


int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }

  //parse
  user_input = argv[1];
  token = tokenize();
  program();

  printf("LI 6,1\n");
  printf("SLL 6,12\n"); // rsp = 4096
  printf("PUSH 5\n");
  printf("MOV 5,6\n");
  printf("SUBI 6,26\n");

  // generate code
  for (int i = 0; code[i] != NULL; i++) {
    gen(code[i]);
    printf("POP 0\n");
  }

  printf("MOV 6,5\n");
  printf("POP 0\n");
  printf("HLT\n");

  return 0;
}
