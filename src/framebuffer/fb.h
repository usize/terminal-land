#pragma once

struct framebuffer {
  int width;
  int height;
  char** buffer;
};

typedef struct framebuffer* framebuffer_t;

framebuffer_t new_framebuffer(int width, int height);
void set_pixel(framebuffer_t fb, int x, int y);
void set_pixel_with_color(framebuffer_t fb, int x, int y, const char* colorcode);
void unset_pixel(framebuffer_t fb, int x, int y);
void draw_fb(framebuffer_t fb);
void clear_fb(framebuffer_t fb);
