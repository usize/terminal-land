#include "graphics.h"

void Graphics_blit(const ImageBuffer_t *fb) {
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      ImageBufferPixel_t p = fb->buffer[(y *fb->width) + x];
			if (p.colorcode < 0) continue;
			attron(COLOR_PAIR(p.colorcode));
			mvaddch(y, x, p.c);
			attroff(COLOR_PAIR(p.colorcode));
    }
  }
}

void Graphics_draw_line(ImageBuffer_t *fb, int x1, int y1, int x2, int y2, int color) {
  // y = mx + b
  float m = (float)(y2 - y1) / (float)(x2 - x1);
  int b = y2 - (int)(x2 * m);
 
  int x_step = 1;
  if (x1 > x2) {
    x_step = -1;
  }
  
  int y_step = 1;
  if (y1 > y2) {
    y_step = -1;
  }
  
  // When m is infinite we take special measures.
  if (m == 1.0/0.0 || m == -1.0/0.0) {
    for (int steps = 0; steps < abs(y1 -y2); steps++) {
      int y = y1 + (steps * y_step);
      ImageBuffer_set_pixel_with_color(fb, x1, y, color);
    }
    return;
  }

  int last_y = y1;
  for (int steps = 0; steps < abs(x2 - x1); steps++) {
    int x = x1 + (steps * x_step);
    int last_y = (int)(x * m) + b;
    ImageBuffer_set_pixel_with_color(fb, x, last_y, color);
  }

  for (int steps = 0; steps < abs(y2 - last_y); steps++) {
    int y = last_y + (steps * y_step); 
    int x = (y - b) / m;
    ImageBuffer_set_pixel_with_color(fb, x, y, color);
    y = y + y_step; 
  }
}

void Graphics_draw_text(ImageBuffer_t *fb, int x, int y, const char *text, int size) {
  for (int i = 0; i < size; i++) {
    char c = text[i];
    if (c == '\0') break;
    ImageBuffer_set_char(fb, x + i, y, c);
  }
}

void Graphics_blit_to_ib(
    const ImageBuffer_t* from, ImageBuffer_t* to, int x_offset, int y_offset) {
  int start_at_x = x_offset;
  int end_at_x = x_offset + from->width;
  int start_at_y = y_offset;
  int end_at_y = y_offset + from->height;
  if (start_at_x >= to->width || start_at_x < 0) return;
  if (start_at_y >= to->height || start_at_y < 0) return;
  if (end_at_x > to->width) {
    end_at_x = to->width; 
  }
  if (end_at_y > to->height) {
    end_at_y = to->height; 
  }
  int copied_width = end_at_x - start_at_x;
  int copied_height = end_at_y - start_at_y;
  for (int y = 0; y < copied_height; y++) {
    for (int x = 0; x < copied_width; x++) {
      ImageBufferPixel_t p = from->buffer[(y * from->width) + x];
      if (p.colorcode > 0) to->buffer[((start_at_y + y) * to->width) + start_at_x + x] = p;
    } 
  }
}
