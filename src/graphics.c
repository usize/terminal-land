#include "graphics.h"

void blit(framebuffer_t fb) {
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      printf("%s", fb->buffer[(y * fb->width) + x]);
    }
    printf("\n");
  }
}

void draw_line(framebuffer_t fb, int x1, int y1, int x2, int y2) {
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
      fb_set_pixel(fb, x1, y);
    }
    return;
  }

  int last_y = y1;
  for (int steps = 0; steps < abs(x2 - x1); steps++) {
    int x = x1 + (steps * x_step);
    int last_y = (int)(x * m) + b;
    fb_set_pixel(fb, x, last_y);
  }

  for (int steps = 0; steps < abs(y2 - last_y); steps++) {
    int y = last_y + (steps * y_step); 
    int x = (y - b) / m;
    fb_set_pixel(fb, x, y);
    y = y + y_step; 
  }
}

