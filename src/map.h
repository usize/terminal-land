#pragma once

#include "stdlib.h"

#define MAP_HEIGHT 1024
#define MAP_WIDTH 4096

enum terrain {
  empty,
  dirt,
  grass
};

struct map {
  int min_x;
  int min_y;
  int max_x;
  int max_y;
  enum terrain data[MAP_HEIGHT * MAP_WIDTH]; 
};

typedef struct map* map_t;

map_t new_map();
