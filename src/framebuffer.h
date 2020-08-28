#pragma once

#include "heap.h"
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
#define FB_COLOR_MAGENTA 6
#define FB_COLOR_YELLOW 7


typedef struct {
  int width;
  int height;
  int* buffer;
} Framebuffer_t;

Framebuffer_t* Framebuffer_new(int width, int height);
void FB_set_pixel(Framebuffer_t *fb, int x, int y);
void FB_set_pixel_with_color(Framebuffer_t *fb, int x, int y, int colorcode);
void FB_unset_pixel(Framebuffer_t *fb, int x, int y);
void FB_clear(Framebuffer_t *fb);
