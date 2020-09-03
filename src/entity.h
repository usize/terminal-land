#pragma once

#include "heap.h"
#include "image_buffer.h"

#define MAX_ENTITIES 1024 
#define MAX_IMAGE_SIZE 64 
#define MAX_IMAGE_FRAMES 1

typedef struct {
  int framecount;
  int current_frame;
  ImageBuffer_t *frames[MAX_IMAGE_FRAMES];
} ImageComponent_t;

typedef struct{
  int x;
  int y;
} PositionComponent_t;

typedef struct {
  int id;
  bool destroyed;
  PositionComponent_t position;
  ImageComponent_t image;
} Entity_t;

typedef int entity_id;

entity_id Entity_create();
Entity_t* Entity_get(entity_id);
void Entity_destroy(entity_id);
Entity_t* Entity_iterator();
void Entity_clear_iterator();
