#pragma once

#include "stdlib.h"
#include "stdio.h"

// color codes

#define FB_COLOR_BLACK  "\033[0;30m██\x1b[0m"
#define FB_COLOR_RED  "\033[0;31m██\x1b[0m"
#define FB_COLOR_GREEN  "\033[0;32m██\x1b[0m"
#define FB_COLOR_BROWN  "\033[0;33m██\x1b[0m"
#define FB_COLOR_BLUE  "\033[0;34m██\x1b[0m"
#define FB_COLOR_PURPLE  "\033[0;35m██\x1b[0m"
#define FB_COLOR_CYAN  "\033[0;36m██\x1b[0m"
#define FB_COLOR_LIGHT_GRAY  "\033[0;37m██\x1b[0m"
#define FB_COLOR_DARK_GRAY  "\033[1;30m██\x1b[0m"
#define FB_COLOR_LIGHT_RED  "\033[1;31m██\x1b[0m"
#define FB_COLOR_LIGHT_GREEN  "\033[1;32m██\x1b[0m"
#define FB_COLOR_YELLOW  "\033[1;33m██\x1b[0m"
#define FB_COLOR_LIGHT_BLUE  "\033[1;34m██\x1b[0m"
#define FB_COLOR_LIGHT_PURPLE  "\033[1;35m██\x1b[0m"
#define FB_COLOR_LIGHT_CYAN  "\033[1;36m██\x1b[0m"
#define FB_COLOR_LIGHT_WHITE  "\033[1;37m██\x1b[0m"
#define FB_COLOR_RESET   "\x1b[0m"
#define PIXEL "██"

struct framebuffer {
  int width;
  int height;
  char** buffer;
};

typedef struct framebuffer* framebuffer_t;

framebuffer_t new_framebuffer(int width, int height);
void fb_set_pixel(framebuffer_t fb, int x, int y);
void fb_set_pixel_with_color(framebuffer_t fb, int x, int y, const char* colorcode);
void fb_unset_pixel(framebuffer_t fb, int x, int y);
void fb_clear(framebuffer_t fb);
