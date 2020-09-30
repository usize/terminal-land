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
 
  // Setup a 32x32 drawing area
  Graphics_draw_text(screen_ptr, 0, 0, "wasd to move, c to change color, q to quit, s to save", 80);

  struct timespec frametime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

  bool blink = true;
  int blink_count, cursor_x, cursor_y, cursor_color = 0;

  // start and stop the game loop.
  bool running = true;
  while (running) {
    // Dynamic screen resizing.
    int check_max_x, check_max_y = 0;
    getmaxyx(stdscr, check_max_y, check_max_x);
    if (check_max_x != max_x || check_max_y != max_y)
      screen_ptr = ImageBuffer_new(check_max_x, check_max_y);

    // Input handling.
    char input_c = getch();
    if (input_c == 'w') {
      clear();
      cursor_y--; 
    } else if (input_c == 's') {
      clear();
      cursor_y++; 
    } else if (input_c == 'a') {
      clear();
      cursor_x--; 
    } else if (input_c == 'd') {
      clear();
      cursor_x++; 
    } else if (input_c == 'c') {
      clear();
      cursor_color++ % 8; 
    } else if (input_c == ' ') {
      clear();
      ImageBuffer_set_pixel_with_color(screen_ptr, cursor_x, cursor_y, cursor_color);
    } else if (input_c == 'q') {
      running = false; 
    }

    attron(COLOR_PAIR(blink ? ImageBuffer_WHITE : cursor_color));
    mvaddch(cursor_y, cursor_x, ' ');
    attron(COLOR_PAIR(blink ? ImageBuffer_WHITE : cursor_color));
    blink_count++;
    if (blink_count >= FPS >> 1) {
      blink = !blink;
      blink_count = 0;
    }
    // Rendering 
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&frametime, NULL);
  }
  return 0;
}
