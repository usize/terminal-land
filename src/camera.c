#include "camera.h"
#include "logging.h"

static void _map_terrain_to_fb(const Map_t *m, int map_x, int map_y, ImageBuffer_t *fb, int fb_x, int fb_y) {
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

void Camera_draw(const Camera_t *c, const Map_t *m, ImageBuffer_t *fb) {
  int half_fb_width = fb->width >> 1;
  int half_fb_height = fb->height >> 1;

  int from_x = c->x - half_fb_width;
  int to_x = c->x + half_fb_width;
  int from_y = c->y - half_fb_height;
  int to_y = c->y + half_fb_height;

  int fb_y = fb->height;
  for (int i = from_y; i < to_y; i++) {
    fb_y--;
    int fb_x = 0;
    for (int j = from_x; j < to_x; j++) {
      _map_terrain_to_fb(m, j, i, fb, ++fb_x, fb_y);
    } 
  }

  bool write_log = false;
  for (Entity_t *e = Entity_iterator(); e != NULL; e = Entity_iterator()) {
    if (e->position.x >= from_x &&
        e->position.x <= to_x &&
        e->position.y >= from_y &&
        e->position.y <= to_y) {
      if (write_log) {
        char ll[2048];
        sprintf(
            (char *)&ll, 
            "from_x: %i from_y %i e_x: %i e_y: %i\n", 
            from_x, from_y, e->position.x, e->position.y);
        LOG(ll);
        write_log = false;
      }
      Graphics_blit_to_ib(
          e->image.frames[0],
          fb, 
          e->position.x - from_x,
          // The map and imagebuffer are flipped relative to one another. 
          to_y - e->position.y); 
    }
  }
  Entity_clear_iterator();
}
