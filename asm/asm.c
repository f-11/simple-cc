#include<stdio.h>
#include<stdlib.h>

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("no arg.\n");
    return 1;
  }
  int *ptr;
  int linenum=0;
  char *p = argv[1];
  ptr = (int *)malloc(1024);
  free(ptr);
  return 0;
}
