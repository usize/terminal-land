#pragma once 

#include "map.h"
#include "image_buffer.h"


typedef struct {
  int map_x;
  int map_y;
} Camera_t;


void Camera_draw(Camera_t*, Map_t*, ImageBuffer_t*);
