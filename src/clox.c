#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clox.h"
#include "vm.h"


void run(const char *filePath) {
  printf("Running file %s\n", filePath);
  char *source = readFile(filePath);
  InterpretResult result = interpret(source);
  free(source);

  switch (result) {
    case INTERPRET_COMPILE_ERROR:
      exit(69);
    case INTERPRET_RUNTIME_ERROR:
      exit(70);
    default:
      break;
  }
}

void runInteractive() {
  printf("Running REPL\n");

  char line[1024];
  
  while (true) {
    printf("> ");
    fgets(line, sizeof(line), stdin);
    if (feof(stdin)) {
      printf("\n");  
    }

    interpret(line);
  }
}

char* readFile(const char *filePath) {
  FILE *fptr = fopen(filePath, "r");

  if (fptr == NULL) {
    fprintf(stderr, "Unable to open source file %s. %s\n", filePath, strerror(errno));
    exit(errno);
  }
  
  fseek(fptr, 0L, SEEK_END);
  size_t fileSize = ftell(fptr);
  rewind(fptr);

  char *buffer = malloc(fileSize + 1); // need + 1 so we can add on the terminator
  
  if (buffer == NULL) {
    fprintf(stderr, "Unable to allocate enough memory to read file %s.\n", filePath);
    exit(74);
  }
        
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, fptr);
  
  if (ferror(fptr)) {
    fprintf(stderr, "Unable to read file %s. %s\n", filePath, strerror(errno));
    exit(errno);
  }

  buffer[bytesRead] = '\0';

  fclose(fptr);
  return buffer;
}
