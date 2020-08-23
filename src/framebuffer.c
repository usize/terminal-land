#include "framebuffer.h"

void static _set_pixel(framebuffer_t fb, int x, int y, int colorcode) {
  if (x < 0 || x >= fb->width) {
    return;
  }
  if (y < 0 || y >= fb->height) {
    return;
  }
	fb->buffer[(y * fb->width) + x] = colorcode;
}

void fb_set_pixel(framebuffer_t fb, int x, int y) {
  _set_pixel(fb, x, y, 1);
}

void fb_set_pixel_with_color(framebuffer_t fb, int x, int y, int colorcode) {
  _set_pixel(fb, x, y, colorcode);
}

void fb_unset_pixel(framebuffer_t fb, int x, int y) {
  fb->buffer[(y * fb->width) + x] = -1;
}

void fb_clear(framebuffer_t fb) {
	memset(fb->buffer, -1, sizeof(int) * fb->width * fb->height);
}

framebuffer_t new_framebuffer(int width, int height) {
  framebuffer_t fb = (framebuffer_t)malloc(sizeof(struct framebuffer));
  fb->width = width;
  fb->height = height;
	int buffer[height * width * 2];
  fb->buffer = (int *)&buffer;
	fb_clear(fb);	
  return fb;
}

