#include "camera.h"
#include "framebuffer.h"
#include "graphics.h"
#include "heap.h"
#include "map.h"

#include "ncurses.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FPS 16


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
  Framebuffer_t *screen_ptr = Framebuffer_new(max_x, max_y);
 
  // Setup map and camera
  Map_t *map_ptr = Map_new();
  Camera_t *camera_ptr = (Camera_t*)(_allocate(sizeof(Camera_t)));
  camera_ptr->map_x = 0;
  camera_ptr->map_y = 0;
  
  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};
  while (1) {
    int check_max_x, check_max_y = 0;
    getmaxyx(stdscr, check_max_y, check_max_x);
    if (check_max_x != max_x || check_max_y != max_y)
      screen_ptr = Framebuffer_new(check_max_x, check_max_y);

    switch(getch()) {
      case 'w':
        camera_ptr->map_y++;
        break;
      case 's':
        camera_ptr->map_y--;
        break;
      case 'a':
        camera_ptr->map_x--;
        break;
      case 'd':
        camera_ptr->map_x++;
        break;
    }
    Camera_draw(camera_ptr, map_ptr, screen_ptr);
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&waittime, NULL);
		FB_clear(screen_ptr);
		//clear();
  }
}
