#include "map.h"

map_t new_map() {
  map_t m = (map_t)malloc(sizeof(struct map));
  m->min_x = (MAP_WIDTH >> 1) * -1;
  m->min_y = (MAP_HEIGHT >> 1) * -1; 
  m->max_x = MAP_WIDTH >> 1;
  m->max_y = MAP_HEIGHT >> 1;
  for (int i = 0; i < MAP_WIDTH * (MAP_HEIGHT >> 1); i++) {
    m->data[i] = dirt; 
  }
  return m;
}
