#include "camera.h"
#include "entity.h"
#include "graphics.h"
#include "heap.h"
#include "image_buffer.h"
#include "map.h"
#include "logging.h"

#include "ncurses.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FPS 60


int main(int argc, char** argv) {
  OPEN_LOG("/tmp/terminal-land.log1");

  // Setup our ncurses screen
  initscr();
  start_color();
  noecho();
  cbreak();
  timeout(1);
  curs_set(FALSE);

  init_pair(ImageBuffer_COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(ImageBuffer_COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
  init_pair(ImageBuffer_COLOR_RED, COLOR_RED, COLOR_RED);
  init_pair(ImageBuffer_COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);        
  init_pair(ImageBuffer_COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);        
  init_pair(ImageBuffer_COLOR_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(ImageBuffer_COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(ImageBuffer_COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);

  // Setup a main window
  int max_x, max_y = 0;
  getmaxyx(stdscr, max_y, max_x);
  ImageBuffer_t *screen_ptr = ImageBuffer_new(max_x, max_y);
 
  // Setup map and camera
  Map_t *map_ptr = Map_new();
  Camera_t *camera_ptr = (Camera_t*)(_allocate(sizeof(Camera_t)));
  camera_ptr->x = 0;
  camera_ptr->y = 0;
  
  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

  Entity_t *player = NULL;
  for (int i = 0; i < 1; i++) {
    entity_id id = Entity_create();
    Entity_t *e = Entity_get(id);
    if (player == NULL) {
      player = e;
    }
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 4, 3, ImageBuffer_COLOR_YELLOW);
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 3, 2, ImageBuffer_COLOR_RED);
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 2, 1, ImageBuffer_COLOR_MAGENTA);
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 0, 3, ImageBuffer_COLOR_YELLOW);
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 1, 2, ImageBuffer_COLOR_RED);
    ImageBuffer_set_pixel_with_color(
        e->image.frames[0], 2, 1, ImageBuffer_COLOR_MAGENTA);
  }

  player->position.x = 25;

  bool running = true;
  while (running) {
    int check_max_x, check_max_y = 0;
    getmaxyx(stdscr, check_max_y, check_max_x);
    if (check_max_x != max_x || check_max_y != max_y)
      screen_ptr = ImageBuffer_new(check_max_x, check_max_y);

    switch(getch()) {
      case 'w':
        player->position.y++;
        camera_ptr->y++;
        break;
      case 's':
        player->position.y--;
        camera_ptr->y--;
        break;
      case 'a':
        player->position.x--;
        if (player->position.x < (camera_ptr->x - max_x / 4)) {
          camera_ptr->x--;
        }
        break;
      case 'd':
        player->position.x++;
        if (player->position.x > (camera_ptr->x + max_x / 4)) {
          camera_ptr->x++;
        }
        break;
      case 'q':
        running = false;
        break;
    }
    Camera_draw(camera_ptr, map_ptr, screen_ptr);
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&waittime, NULL);
		ImageBuffer_clear(screen_ptr);
		//clear();
  }

  CLOSE_LOG();
}
