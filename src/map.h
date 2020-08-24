#pragma once

#include "framebuffer.h"
#include "string.h"
#include <stdlib.h>

#define MAP_HEIGHT 1024
#define MAP_WIDTH 4096

enum terrain {
  empty,
  dirt,
  grass,
  sky,
};

struct map {
  int mid_x;
  int mid_y;
  int min_x;
  int min_y;
  int max_x;
  int max_y;
  int data[MAP_HEIGHT * MAP_WIDTH]; 
};

typedef struct map* map_t;

map_t new_map();

enum terrain map_get_data(map_t m_t, int x, int y);
