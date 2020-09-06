#pragma once

#include "entity.h"
#include "gamecontext.h"

#define MAX_EVENTS 128
#define N_EVENT_TYPES 3

typedef enum {
  // Request events are sent to a server on behalf of a client.
  REQUEST_ENTITY_CREATE,
  REQUEST_ENTITY_DESTROY,
  REQUEST_ENTITY_MOVE,
  ENTITY_CREATE,
  ENTITY_DESTROY,
  ENTITY_MOVE,
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
} EntityMoveEvent;

typedef union {
  EntityCreateEvent entity_create;
  EntityDestroyEvent entity_destroy;
  EntityMoveEvent entity_move;
} _event;

typedef struct {
  event_type _type;
  _event data;
} Event_t;

typedef int (*event_handler)(GameContext_t*, Event_t);

typedef struct {
  Event_t queue[MAX_EVENTS];
  int queue_top;
  event_handler handlers[N_EVENT_TYPES];
} EventBus_t;

EventBus_t* EventBus_new();
void EventBus_push(Event_t);
Event_t EventBus_pop();
void EventBus_register_handler(event_type, event_handler);
