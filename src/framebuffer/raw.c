#include "raw.h"

framebuffer_t new_framebuffer(int width, int height) {
  framebuffer_t fb = (framebuffer_t)malloc(sizeof(struct framebuffer));
  fb->width = width;
  fb->height = height;
  fb->buffer = (char**)malloc(sizeof(width * height * sizeof(char*)));
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      fb->buffer[(y * fb->width) + x] = (char*)malloc(sizeof(char[1024]));
      fb->buffer[(y * fb->width) + x] = " ";
    }
  }
  return fb;
}

void static _set_pixel(framebuffer_t fb, int x, int y, const char* colorcode) {
	if (x < 0 || x >= fb->width) {
		return;
	}
	if (y < 0 || y >= fb->height) {
		return;
	}
	if (colorcode == NULL) {
		fb->buffer[(y * fb->width) + x] = PIXEL;
	} else {
		fb->buffer[(y * fb->width) + x] = colorcode;
	}
}

void set_pixel(framebuffer_t fb, int x, int y) {
	_set_pixel(fb, x, y, NULL);
}

void set_pixel_with_color(framebuffer_t fb, int x, int y, const char* colorcode) {
	_set_pixel(fb, x, y, colorcode);
}

void unset_pixel(framebuffer_t fb, int x, int y) {
  fb->buffer[(y * fb->width) + x] = "  ";
}

void draw_fb(framebuffer_t fb) {
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      printf("%s", fb->buffer[(y * fb->width) + x]); 
    }
    printf("\n");
  }
}

void clear_fb(framebuffer_t fb) {
  for (int y = 0; y < fb->height; y++) {
    for (int x = 0; x < fb->width; x++) {
      unset_pixel(fb, x, y); 
    }
  }
}
