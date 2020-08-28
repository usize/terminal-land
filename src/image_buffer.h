#pragma once

#include "heap.h"
#include "stdio.h"
#include "string.h"
#include "ncurses.h"

// color codes

#define ImageBuffer_COLOR_BLACK  0
#define ImageBuffer_COLOR_WHITE  1
#define ImageBuffer_COLOR_RED  2
#define ImageBuffer_COLOR_GREEN  3
#define ImageBuffer_COLOR_BLUE  4
#define ImageBuffer_COLOR_CYAN  5
#define ImageBuffer_COLOR_MAGENTA 6
#define ImageBuffer_COLOR_YELLOW 7


typedef struct {
  int width;
  int height;
  int* buffer;
} ImageBuffer_t;

ImageBuffer_t* ImageBuffer_new(int width, int height);
void ImageBuffer_set_pixel(ImageBuffer_t *fb, int x, int y);
void ImageBuffer_set_pixel_with_color(ImageBuffer_t *fb, int x, int y, int colorcode);
void ImageBuffer_unset_pixel(ImageBuffer_t *fb, int x, int y);
void ImageBuffer_clear(ImageBuffer_t *fb);
