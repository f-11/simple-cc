#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の個数が不正です。\n");
    return 1;
  }
  return 0;
}
