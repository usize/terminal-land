#pragma once 

#include "entity.h"
#include "graphics.h"
#include "map.h"
#include "image_buffer.h"

#define FOLLOW_LAG 35

typedef struct {
  int x;
  int y;
} Camera_t;

void Camera_follow(Camera_t*, const Entity_t*);
void Camera_draw(const Camera_t*, const Map_t*, EntityPool_t*, ImageBuffer_t*);
