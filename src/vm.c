#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

VM vm;

static void resetStack() { vm.stackTop = vm.stack; }

void initVM() { resetStack(); }

void freeVM() {}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.pc++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do { \
    double left = pop(); \
    double right = pop(); \
    push(left op right); \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("stack:  ");
    for (Value *slot = vm.stack; slot < vm.stackTop; ++slot) {
      printf("[");
      printValue(*slot);
      printf("]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.pc - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_RETURN: {
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
      break;
    }
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_NEGATE: {
      push(-pop());
      break;
    }
    case OP_ADD: {
      BINARY_OP(+);
      break;
    }
    case OP_SUBTRACT: {
      BINARY_OP(-);
      break;
    }
    case OP_MULTIPLY: {
      BINARY_OP(*);
      break;
    }
    case OP_DIVIDE: {
      BINARY_OP(/);
      break;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.pc = vm.chunk->code;
  return run();
}
