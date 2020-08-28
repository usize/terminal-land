#include "framebuffer.h"

void static _set_pixel(Framebuffer_t *fb, int x, int y, int colorcode) {
  if (x < 0 || x >= fb->width) {
    return;
  }
  if (y < 0 || y >= fb->height) {
    return;
  }
	fb->buffer[(y * fb->width) + x] = colorcode;
}

void FB_set_pixel(Framebuffer_t *fb, int x, int y) {
  _set_pixel(fb, x, y, 1);
}

void FB_set_pixel_with_color(Framebuffer_t *fb, int x, int y, int colorcode) {
  _set_pixel(fb, x, y, colorcode);
}

void FB_unset_pixel(Framebuffer_t *fb, int x, int y) {
  fb->buffer[(y * fb->width) + x] = -1;
}

void FB_clear(Framebuffer_t *fb) {
	memset(fb->buffer, -1, sizeof(int) * fb->width * fb->height);
}

Framebuffer_t* Framebuffer_new(int width, int height) {
  Framebuffer_t *fb = (Framebuffer_t*)_allocate(sizeof(Framebuffer_t));
  fb->width = width;
  fb->height = height;
	int buffer[height * width * 2];
  fb->buffer = (int *)&buffer;
	FB_clear(fb);	
  return fb;
}

