#pragma once 

#include "entity.h"
#include "graphics.h"
#include "map.h"
#include "image_buffer.h"


typedef struct {
  int x;
  int y;
} Camera_t;


void Camera_draw(Camera_t*, Map_t*, ImageBuffer_t*);
