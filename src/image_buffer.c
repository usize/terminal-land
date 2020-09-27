#include "image_buffer.h"

void static _set_pixel(ImageBuffer_t *fb, int x, int y, int colorcode) {
  if (x < 0 || x >= fb->width) {
    return;
  }
  if (y < 0 || y >= fb->height) {
    return;
  }
	fb->buffer[(y * fb->width) + x] = colorcode;
}

void ImageBuffer_set_pixel(ImageBuffer_t *fb, int x, int y) {
  _set_pixel(fb, x, y, 1);
}

void ImageBuffer_set_pixel_with_color(ImageBuffer_t *fb, int x, int y, int colorcode) {
  _set_pixel(fb, x, y, colorcode);
}

void ImageBuffer_unset_pixel(ImageBuffer_t *fb, int x, int y) {
  fb->buffer[(y * fb->width) + x] = -1;
}

void ImageBuffer_clear(ImageBuffer_t *fb) {
	memset(fb->buffer, -1, sizeof(int) * fb->width * fb->height);
}

ImageBuffer_t* ImageBuffer_new(int width, int height) {
  ImageBuffer_t *fb = (ImageBuffer_t*)malloc(sizeof(*fb));
  fb->width = width;
  fb->height = height;
  fb->buffer = (int *)malloc(sizeof(int) * width * height);
	ImageBuffer_clear(fb);	
  return fb;
}

