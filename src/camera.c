#include "camera.h"

static void _map_terrain_to_fb(Map_t *m, int map_x, int map_y, ImageBuffer_t *fb, int fb_x, int fb_y) {
  switch (Map_get_data(m, map_x, map_y)) {
    case dirt:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_COLOR_YELLOW);
      break;
    case sky:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_COLOR_BLUE);
      break;
    case grass:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_COLOR_GREEN);
      break;
    default:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_COLOR_WHITE);
  }
}

void Camera_draw(Camera_t *c, Map_t *m, ImageBuffer_t *fb) {
  int half_fb_width = fb->width >> 1;
  int half_fb_height = fb->height >> 1;

  int from_x = c->map_x - half_fb_width;
  int to_x = c->map_x + half_fb_width;
  int from_y = c->map_y - half_fb_height;
  int to_y = c->map_y + half_fb_height;

  int fb_y = fb->height;
  for (int i = from_y; i < to_y; i++) {
    fb_y--;
    int fb_x = 0;
    for (int j = from_x; j < to_x; j++) {
      _map_terrain_to_fb(m, j, i, fb, ++fb_x, fb_y);
    } 
  }

}
