#include "image_buffer.h"

void static _set_pixel(ImageBuffer_t *fb, int x, int y, int colorcode) {
  if (x < 0 || x >= fb->width) {
    return;
  }
  if (y < 0 || y >= fb->height) {
    return;
  }
  ImageBufferPixel_t p = {.colorcode=colorcode, .c=' '};
	fb->buffer[(y * fb->width) + x] = p;
}

void ImageBuffer_set_char(ImageBuffer_t *fb, int x, int y, char c) {
  if (x < 0 || x >= fb->width) {
    return;
  }
  if (y < 0 || y >= fb->height) {
    return;
  }
  ImageBufferPixel_t p = {.colorcode=ImageBuffer_WHITE_ON_BLACK, .c=c};
	fb->buffer[(y * fb->width) + x] = p;
}

void ImageBuffer_set_pixel(ImageBuffer_t *fb, int x, int y) {
  _set_pixel(fb, x, y, 1);
}

void ImageBuffer_set_pixel_with_color(ImageBuffer_t *fb, int x, int y, int colorcode) {
  _set_pixel(fb, x, y, colorcode);
}

void ImageBuffer_unset_pixel(ImageBuffer_t *fb, int x, int y) {
  ImageBufferPixel_t p = {.colorcode=-1, .c=' '};
  fb->buffer[(y * fb->width) + x] = p;
}

void ImageBuffer_clear(ImageBuffer_t *fb) {
	memset(fb->buffer, -1, sizeof(ImageBufferPixel_t) * fb->width * fb->height);
}

ImageBuffer_t* ImageBuffer_new(int width, int height) {
  ImageBuffer_t *fb = (ImageBuffer_t*)malloc(sizeof(*fb));
  fb->width = width;
  fb->height = height;
  fb->buffer = (ImageBufferPixel_t *)malloc(sizeof(ImageBufferPixel_t) * width * height);
	ImageBuffer_clear(fb);	
  return fb;
}

