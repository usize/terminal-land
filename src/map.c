#include "map.h"

map_t new_map() {
  map_t m = (map_t)_allocate(sizeof(struct map));
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

enum terrain map_get_data(map_t m_t, int x, int y) {
  if (x < m_t->min_x || x > m_t->max_x) return empty;
  if (y < m_t->min_y || y > m_t->max_y) return empty;
  return m_t->data[((m_t->mid_y + y) * MAP_WIDTH) + (m_t->mid_x + x)];
}
