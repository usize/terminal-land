#pragma once

#include "image_buffer.h"
#include "string.h"
#include "stdlib.h"

#define MAP_HEIGHT 512
#define MAP_WIDTH 1024

enum Terrain {
  empty,
  dirt,
  grass,
  sky,
};

typedef struct {
  int mid_x;
  int mid_y;
  int min_x;
  int min_y;
  int max_x;
  int max_y;
  int data[MAP_HEIGHT * MAP_WIDTH]; 
} Map_t;

Map_t* Map_new();

enum Terrain Map_get_data(const Map_t *m_t, int x, int y);
