#pragma once

#include "stdio.h"
#include "string.h"
#include "ncurses.h"
#include "stdlib.h"

// color codes

#define ImageBuffer_BLACK  0
#define ImageBuffer_WHITE  1
#define ImageBuffer_RED  2
#define ImageBuffer_GREEN  3
#define ImageBuffer_BLUE  4
#define ImageBuffer_CYAN  5
#define ImageBuffer_MAGENTA 6
#define ImageBuffer_YELLOW 7
#define ImageBuffer_WHITE_ON_BLACK 8

typedef struct {
  int colorcode;
  char c;
} ImageBufferPixel_t;

typedef struct {
  int width;
  int height;
  ImageBufferPixel_t *buffer;
} ImageBuffer_t;

ImageBuffer_t* ImageBuffer_new(int width, int height);
void ImageBuffer_set_pixel(ImageBuffer_t *fb, int x, int y);
void ImageBuffer_set_char(ImageBuffer_t *fb, int x, int y, char c);
void ImageBuffer_set_pixel_with_color(ImageBuffer_t *fb, int x, int y, int colorcode);
void ImageBuffer_unset_pixel(ImageBuffer_t *fb, int x, int y);
void ImageBuffer_clear(ImageBuffer_t *fb);
