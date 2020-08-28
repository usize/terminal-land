#pragma once

#include "framebuffer.h"

typedef struct {
  Framebuffer_t fb[8];
} EntitySprite_t;

typedef struct {
  int x;
  int y;
} EntityPosition_t;

typedef struct {
  int id;
  EntityPosition_t position; 
  EntitySprite_t sprite;
} Entity_t;
