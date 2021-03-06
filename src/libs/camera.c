#include "camera.h"

static void _map_terrain_to_fb(const Map_t *m, int map_x, int map_y, ImageBuffer_t *fb, int fb_x, int fb_y) {
  switch (Map_get_data(m, map_x, map_y)) {
    case dirt:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_YELLOW);
      break;
    case sky:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_BLUE);
      break;
    case grass:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_GREEN);
      break;
    default:
      ImageBuffer_set_pixel_with_color(fb, fb_x, fb_y, ImageBuffer_WHITE);
  }
}

void Camera_draw(const Camera_t *c, const Map_t *m, EntityPool_t *ep, ImageBuffer_t *fb) {
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

  EntityPool_clear_iterator(ep);
  for (Entity_t *e = EntityPool_iterator(ep); e != NULL; e = EntityPool_iterator(ep)) {
    if (e->position.x >= from_x &&
        e->position.x <= to_x &&
        e->position.y >= from_y &&
        e->position.y <= to_y) {
      Graphics_blit_to_ib(
          ep->images[e->image.ids[0]],
          fb, 
          e->position.x - from_x,
          // The map and imagebuffer are flipped relative to one another. 
          to_y - e->position.y,
          e->image.flipped);

      char label[14];
      sprintf(label, " entity: %i ", e->id);
      Graphics_draw_text(fb, e->position.x - from_x, to_y - e->position.y, label, 12);
    }
  }
  EntityPool_clear_iterator(ep);
}

void Camera_follow(Camera_t *c, const Entity_t *e) {
  if (c->x > e->position.x) {
    while ((c->x - e->position.x) > FOLLOW_LAG_X) {
      c->x--;
    }  
  } else {
    while ((e->position.x - c->x) > FOLLOW_LAG_X) {
      c->x++; 
    }
  }
  if (c->y > e->position.y) {
    while ((c->y - e->position.y) > FOLLOW_LAG_Y) {
      c->y--;
    }  
  } else {
    while ((e->position.y - c->y) > FOLLOW_LAG_Y) {
      c->y++; 
    }
  }
}
