#pragma once

#include "image_buffer.h"
#include "graphics.h"

#include "stdlib.h"

#define MAX_ENTITIES 256 
#define MAX_IMAGE_SIZE 32
#define MAX_IMAGE_FRAMES 8
#define MAX_IMAGES 512

typedef int entity_id;

typedef struct {
  int framecount;
  int current_frame;
  int ids[MAX_IMAGE_FRAMES];
} ImageComponent_t;

typedef struct{
  int x;
  int y;
} PositionComponent_t;

typedef struct {
  entity_id id;
  bool destroyed;
  PositionComponent_t position;
  ImageComponent_t image;
} Entity_t;

typedef struct {
  Entity_t entities[MAX_ENTITIES];
  ImageBuffer_t* images[MAX_IMAGES];
  int nonce;
  int entities_iterator;
} EntityPool_t;

entity_id Entity_create(EntityPool_t*);
Entity_t* Entity_get(EntityPool_t*, entity_id);
void Entity_destroy(EntityPool_t*, entity_id);
Entity_t* EntityPool_iterator(EntityPool_t*);
void EntityPool_push(EntityPool_t*, const Entity_t);
void EntityPool_clear_iterator(EntityPool_t*);
EntityPool_t* EntityPool_create();
void EntityPool_destroy(EntityPool_t*);
