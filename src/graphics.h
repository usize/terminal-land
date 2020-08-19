#pragma once

#include "ncurses.h"
#include "framebuffer.h"

void blit(framebuffer_t fb);
void draw_line(framebuffer_t fb, int x1, int y1, int x2, int y2);

