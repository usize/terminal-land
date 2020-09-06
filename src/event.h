#pragma once

#include "entity.h"

#define MAX_EVENTS = 128

typedef enum {
  ENTITY_CREATE,
  ENTITY_DESTROY,
  ENTITY_POSITION_CHANGE,
} event_type;

typedef struct {
  entity_id id;
  int x;
  int y;
} EntityCreateEvent;

typedef struct {
  entity_id id;
} EntityDestroyEvent;

typedef struct {
  entity_id id;
} EntityPositionChangeEvent;

typedef union {
  EntityCreateEvent entity_create;
  EntityDestroyEvent entity_destroy;
  EntityPositionChangeEvent entity_position_change;
} _event;

typedef struct {
  event_type _type;
  _event data;
} Event_t;

typedef int (*event_handler)(Event_t);
