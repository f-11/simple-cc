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

int q;
int get_unique_num() {
  q++;
  return q;
}

char *get_unique_str(const char *startwith) {
  int len = strlen(startwith);
  char *ptr = calloc(len + 5, sizeof(char));  //MEMO: unique number が４桁になったら終わる
  strcpy(ptr, startwith);
  sprintf(ptr+len, ".%d", get_unique_num());
  return ptr;
}

char *get_str(const char *str) {
  int len = strlen(str);
  char *ptr = calloc(len + 1, sizeof(char));
  strcpy(ptr, str);
  return ptr;
}

//============

int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }
  
  Inst dummy_inst;
  Label dummy_label;
  inst = &dummy_inst;
  label = &dummy_label;

  char *mainlabel = get_str("main");

  user_input = argv[1];
  //tokenize
  token = tokenize();
  //parse
  program();

  //initial code
  add_inst(LI, 6, 1);
  add_inst(SLL, 6, 12); // rsp = 4096
  add_push(5);
  add_inst(MOV, 5, 6);
  add_call(mainlabel);
  add_inst(MOV, 6, 5);
  add_pop(0);
  add_inst(HLT);
  add_label(mainlabel);

  // generate code
  for (int i = 0; code[i] != NULL; i++) {
    gen(code[i]);
    add_pop(0);
  }
  add_inst(BR);

  // set distance to B*
  link(dummy_inst.next, dummy_label.next);
  // print code
  print_inst(dummy_inst.next);

  return 0;
}
