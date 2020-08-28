#pragma once 

#include "map.h"
#include "framebuffer.h"


typedef struct {
  int map_x;
  int map_y;
} Camera_t;


void Camera_draw(Camera_t*, Map_t*, Framebuffer_t*);
