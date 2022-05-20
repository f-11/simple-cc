#include "simple-cc.h"

// global var
Token *token;
char *user_input; //入力されたプログラム


int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }

  //parse
  user_input = argv[1];
  token = tokenize(argv[1]);
  Node *node = expr();

  // generate code
  gen(node);

  printf("POP 0\n");
  printf("HLT\n");

  return 0;
}
