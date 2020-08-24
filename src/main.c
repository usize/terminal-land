#include "camera.h"
#include "framebuffer.h"
#include "graphics.h"
#include "map.h"

#include "ncurses.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FPS 16


// A goofy function to test drawing.
static void spirals(framebuffer_t fb, int max_x, int max_y) {
  static double v = 1.0; 
  static double a = 0.0;

  int midpoint_x = fb->width >> 1;
  int midpoint_y = fb->height >> 1;

  fb_clear(fb);
  
  static const int colors[3] = { 
    FB_COLOR_RED,
    FB_COLOR_BLUE,
    FB_COLOR_GREEN,
  };

  static int color_i = 0;
  draw_line(fb, 20, 0, max_x - 20, 0);
  draw_line(fb, 20, 0, 20, max_y - 1);
  draw_line(fb, max_x - 20, 0, max_x - 20, max_y);
  draw_line(fb, 20, max_y - 1, max_x - 20, max_y - 1);

  for (int i = 0; i < 1024 * 2; i++) {
    int c_x = midpoint_x + (int)v*cos(a);
    int c_y = midpoint_y + (int)v*sin(a);
    fb_set_pixel_with_color(fb, c_x, c_y, colors[color_i]);
    a = a + 0.001;
    v = v + 0.001;
    if (a > 2.0 * 3.14159) {
      a = 0.0;
    }
    if (v > max_x / 4) {
      v = 1.0;
      color_i = (color_i + 1) % 3;
    }
  }
}

int main(int argc, char** argv) {
  // Setup our ncurses screen
  initscr();
  start_color();
  noecho();
  cbreak();
  timeout(1);
  curs_set(FALSE);

  init_pair(FB_COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(FB_COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
  init_pair(FB_COLOR_RED, COLOR_RED, COLOR_RED);
  init_pair(FB_COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);        
  init_pair(FB_COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);        
  init_pair(FB_COLOR_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(FB_COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(FB_COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);

  // Setup a main framebuffer
  int max_x, max_y = 0;
  getmaxyx(stdscr, max_y, max_x);
  framebuffer_t fb_t = new_framebuffer(max_x, max_y);
 
  // Setup map and camera
  map_t m_t = new_map();
  camera_t c_t = (camera_t)(malloc(sizeof(struct camera)));
  c_t->map_x = 0;
  c_t->map_y = 0;
  
  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};
  while (1) {
    switch(getch()) {
      case 'w':
        c_t->map_y++;
        break;
      case 's':
        c_t->map_y--;
        break;
      case 'a':
        c_t->map_x--;
        break;
      case 'd':
        c_t->map_x++;
        break;
    }
    camera_draw(c_t, m_t, fb_t);
    blit(fb_t);
    refresh();
    nanosleep(&waittime, NULL);
		fb_clear(fb_t);
		//clear();
  }
}
