#pragma once

#include "map.h"
#include "entity.h"

typedef struct {
  Map_t *map;
  EntityPool_t *entity_pool;
  Entity_t *player;
} GameContext_t;
