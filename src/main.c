#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include <stdio.h>

int main() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);
  writeChunk(&chunk, OP_NEGATE, 123);

  int constant2 = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant2, 123);

  writeChunk(&chunk, OP_ADD, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  interpret(&chunk);
  
  freeChunk(&chunk);
  freeVM();

  return 0;
}
