#pragma once

#include "ncurses.h"
#include "framebuffer.h"

void Graphics_blit(Framebuffer_t *fb);
void Graphics_draw_line(Framebuffer_t *fb, int x1, int y1, int x2, int y2);

