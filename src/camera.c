#include "camera.h"

static void _map_terrain_to_fb(map_t m_t, int map_x, int map_y, framebuffer_t fb_t, int fb_x, int fb_y) {
  switch (map_get_data(m_t, map_x, map_y)) {
    case dirt:
      fb_set_pixel_with_color(fb_t, fb_x, fb_y, FB_COLOR_YELLOW);
      break;
    case sky:
      fb_set_pixel_with_color(fb_t, fb_x, fb_y, FB_COLOR_BLUE);
      break;
    case grass:
      fb_set_pixel_with_color(fb_t, fb_x, fb_y, FB_COLOR_GREEN);
      break;
    default:
      fb_set_pixel_with_color(fb_t, fb_x, fb_y, FB_COLOR_WHITE);
  }
}

void camera_draw(camera_t c_t, map_t m_t, framebuffer_t fb_t) {
  int half_fb_width = fb_t->width >> 1;
  int half_fb_height = fb_t->height >> 1;

  int from_x = c_t->map_x - half_fb_width;
  int to_x = c_t->map_x + half_fb_width;
  int from_y = c_t->map_y - half_fb_height;
  int to_y = c_t->map_y + half_fb_height;

  int fb_y = fb_t->height;
  for (int i = from_y; i < to_y; i++) {
    fb_y--;
    int fb_x = 0;
    for (int j = from_x; j < to_x; j++) {
      _map_terrain_to_fb(m_t, j, i, fb_t, ++fb_x, fb_y);
    } 
  }

}
