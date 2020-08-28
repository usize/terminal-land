#pragma once

#include "ncurses.h"
#include "image_buffer.h"

void Graphics_blit(ImageBuffer_t*);
void Graphics_draw_line(ImageBuffer_t*, int x1, int y1, int x2, int y2);

