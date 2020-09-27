#pragma once

#include "entity.h"
#include "gamecontext.h"

#define MAX_EVENTS 1024
#define N_EVENT_TYPES 3

typedef enum {
  NULL_EVENT,
  ENTITY_CREATE,
  ENTITY_LOAD,
  ENTITY_DESTROY,
  ENTITY_MOVE,
} event_type;

typedef struct {} EntityCreateEvent;

typedef struct {
  Entity_t entity;
} EntityLoadEvent;

typedef struct {
  entity_id id;
} EntityDestroyEvent;

typedef struct {
  entity_id id;
  // It's useful to teleport entities instead of using a delta 
  // in order to keep multiplayer synced. The {from,to}_{x, y}
  // values can be used for collision detection and other sanity checks.
  int from_x;
  int from_y;
  int to_x;
  int to_y;
} EntityMoveEvent;

typedef union {
  EntityCreateEvent entity_create;
  EntityLoadEvent entity_load;
  EntityDestroyEvent entity_destroy;
  EntityMoveEvent entity_move;
} event;

typedef struct {
  event_type type;
  event data;
} Event_t;

typedef int (*event_handler)(Event_t, GameContext_t*);

typedef struct {
  Event_t queue[MAX_EVENTS];
  int queue_top;
  event_handler handlers[N_EVENT_TYPES];
} EventBus_t;

int EventBus_push(EventBus_t*, Event_t);
Event_t EventBus_pop(EventBus_t*);
void EventBus_register_handler(EventBus_t*, event_type, event_handler);
void EventBus_handle_events(EventBus_t*, GameContext_t *ctx);
