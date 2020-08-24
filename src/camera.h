#pragma once 

#include "map.h"
#include "framebuffer.h"


struct camera {
  int map_x;
  int map_y;
};

typedef struct camera* camera_t;

void camera_draw(camera_t c_t, map_t m_t, framebuffer_t fb_t);
