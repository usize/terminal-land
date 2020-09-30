#include "libs/camera.h"
#include "libs/entity.h"
#include "libs/event.h"
#include "libs/gamecontext.h"
#include "libs/graphics.h"
#include "libs/image_buffer.h"
#include "libs/logging.h"
#include "libs/map.h"
#include "libs/networking.h"

#include "ncurses.h"

#include "errno.h"
#include "fcntl.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "strings.h"
#include "time.h"

#define FPS 60

int main(int argc, char** argv) {
  // Setup our ncurses screen
  initscr();
  start_color();
  noecho();
  cbreak();
  timeout(1);
  curs_set(FALSE);

  init_pair(ImageBuffer_BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(ImageBuffer_WHITE, COLOR_WHITE, COLOR_WHITE);
  init_pair(ImageBuffer_RED, COLOR_RED, COLOR_RED);
  init_pair(ImageBuffer_GREEN, COLOR_GREEN, COLOR_GREEN);        
  init_pair(ImageBuffer_BLUE, COLOR_BLUE, COLOR_BLUE);        
  init_pair(ImageBuffer_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(ImageBuffer_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(ImageBuffer_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(ImageBuffer_WHITE_ON_BLACK, COLOR_BLACK, COLOR_WHITE);

  // Setup a main window
  int max_x, max_y = 0;
  getmaxyx(stdscr, max_y, max_x);
  ImageBuffer_t *screen_ptr = ImageBuffer_new(max_x, max_y);
  
  struct timespec frametime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

  // start and stop the game loop.
  bool running = true;
  while (running) {
    // Dynamic screen resizing.
    int check_max_x, check_max_y = 0;
    getmaxyx(stdscr, check_max_y, check_max_x);
    if (check_max_x != max_x || check_max_y != max_y)
      screen_ptr = ImageBuffer_new(check_max_x, check_max_y);

    // Input handling.
    //char input_c = getch(); 
    

    // Rendering.
    Graphics_draw_text(screen_ptr, 1, 0, "press space to add a pixel and c to change colors", 80);
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&frametime, NULL);
    ImageBuffer_clear(screen_ptr);
  
  }
  return 0;
}
