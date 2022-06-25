#include <stdio.h>
#include <stdlib.h>
#include "clox.h"

int main(int argc, char *argsv[]) {
  if (argc == 1) {
    runInteractive();
  }
  else if (argc == 2) {
    run(argsv[1]);
  }
  else {
    fprintf(stderr, "usage 'clox' or  'clox [file path]' \n");  
    return 127;
  }
  return 0;
}
