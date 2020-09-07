#include "camera.h"
#include "entity.h"
#include "event.h"
#include "gamecontext.h"
#include "graphics.h"
#include "image_buffer.h"
#include "map.h"
#include "logging.h"

#include "ncurses.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FPS 60


int local_movement_handler(Event_t ev, GameContext_t* ctx) {
  EntityMoveEvent *move_event = (EntityMoveEvent*)&ev.data;
  entity_id id = move_event->id;
  int delta_x = move_event->delta_x;
  int delta_y = move_event->delta_y;
  Entity_t *e = Entity_get(ctx->entity_pool, id);
  if (e == NULL) {
    return -1;
  }
  e->position.x = e->position.x + delta_x;
  e->position.y = e->position.y + delta_y;
  return 0;
}

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

  // Setup entities.
  EntityPool_t *entity_pool_ptr = EntityPool_create();

  // Setup map and camera
  Map_t *map_ptr = Map_new();
  Camera_t *camera_ptr = (Camera_t*)(malloc(sizeof(Camera_t)));
  camera_ptr->x = 0;
  camera_ptr->y = 0;

  // Setup game context
  GameContext_t ctx = {.map=map_ptr, .entity_pool=entity_pool_ptr};

  // Initialize event handlers
  EventBus_t event_bus;
  event_bus.queue_top = 0;
  EventBus_register_handler(&event_bus, ENTITY_MOVE, local_movement_handler);

  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

  Entity_t *player = NULL;
  for (int i = 0; i < 1; i++) {
    entity_id id = Entity_create(entity_pool_ptr);
    Entity_t *e = Entity_get(entity_pool_ptr, id);
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

    EntityMoveEvent move_right_event = {.id=player->id, .delta_x=1, .delta_y=0};
    EntityMoveEvent move_left_event = {.id=player->id, .delta_x=-1, .delta_y=0};
    EntityMoveEvent move_up_event = {.id=player->id, .delta_x=0, .delta_y=1};
    EntityMoveEvent move_down_event = {.id=player->id, .delta_x=0, .delta_y=-1};
    Event_t ev;
    ev.type = ENTITY_MOVE;
    switch(getch()) {
      case 'w':
        ev.data = (event)move_up_event;
        EventBus_push(&event_bus, ev);
        break;
      case 's':
        ev.data = (event)move_down_event;
        EventBus_push(&event_bus, ev);
        break;
      case 'a':
        ev.data = (event)move_left_event;
        EventBus_push(&event_bus, ev);
        break;
      case 'd':
        ev.data = (event)move_right_event;
        EventBus_push(&event_bus, ev);
        break;
      case 'q':
        running = false;
        break;
    }
    EventBus_handle_events(&event_bus, &ctx);
    Camera_draw(camera_ptr, map_ptr, entity_pool_ptr, screen_ptr);
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&waittime, NULL);
		ImageBuffer_clear(screen_ptr);
		//clear();
  }

  CLOSE_LOG();
  EntityPool_destroy(entity_pool_ptr);
  free(map_ptr);
  free(camera_ptr);
}
