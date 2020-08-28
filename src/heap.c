#include "heap.h"
#include "stdio.h" 

static int loc = 0;
static char heap[HEAP_SIZE];

void* _allocate(size_t n) {
  if (loc > HEAP_SIZE) {
    return NULL;
  }
  void* mem = &heap[loc];
  loc = loc + n + 1;
  return mem;
}
