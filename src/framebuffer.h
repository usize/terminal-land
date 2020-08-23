#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ncurses.h"

// color codes

#define FB_COLOR_BLACK  0
#define FB_COLOR_WHITE  1
#define FB_COLOR_RED  2
#define FB_COLOR_GREEN  3
#define FB_COLOR_BLUE  4
#define FB_COLOR_CYAN  5


struct framebuffer {
  int width;
  int height;
  int* buffer;
};

typedef struct framebuffer* framebuffer_t;

framebuffer_t new_framebuffer(int width, int height);
void fb_set_pixel(framebuffer_t fb, int x, int y);
void fb_set_pixel_with_color(framebuffer_t fb, int x, int y, int colorcode);
void fb_unset_pixel(framebuffer_t fb, int x, int y);
void fb_clear(framebuffer_t fb);
