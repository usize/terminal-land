#pragma once

#include "stdlib.h"

#define HEAP_SIZE 1024 * 1024 * 25

// Our own memory allocator that prevents us from using more than HEAP_SIZE mem.
void* _allocate(size_t);

