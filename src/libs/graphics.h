#pragma once

#include "ncurses.h"
#include "image_buffer.h"
#include "string.h"

void Graphics_blit(const ImageBuffer_t*);
void Graphics_blit_to_ib(const ImageBuffer_t* from, ImageBuffer_t* to, int x_offset, int y_offset, bool flip);
void Graphics_draw_line(ImageBuffer_t*, int x1, int y1, int x2, int y2, int color);
void Graphics_draw_text(ImageBuffer_t*, int x, int y, const char* text, int size);

