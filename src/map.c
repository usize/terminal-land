#include "map.h"

Map_t* Map_new() {
  Map_t *m = (Map_t*)_allocate(sizeof(Map_t));
  m->mid_x = MAP_WIDTH >> 1;
  m->mid_y = MAP_HEIGHT >> 1;
  m->min_x = 0 - (MAP_WIDTH >> 1);
  m->min_y = 0 - (MAP_HEIGHT >> 1); 
  m->max_x = MAP_WIDTH >> 1;
  m->max_y = MAP_HEIGHT >> 1;
  for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
    m->data[i] = sky; 
  }

  // Simple terrain generation.
  for (int i = 0; i < MAP_WIDTH * (MAP_HEIGHT >> 1); i++) {
    int r = rand();
    m->data[i] = r > RAND_MAX >> 1 ? dirt : grass; 
  }
  return m;
}

enum Terrain Map_get_data(const Map_t *m, int x, int y) {
  if (x < m->min_x || x > m->max_x) return empty;
  if (y < m->min_y || y > m->max_y) return empty;
  return m->data[((m->mid_y + y) * MAP_WIDTH) + (m->mid_x + x)];
}
