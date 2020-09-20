#include "camera.h"
#include "entity.h"
#include "event.h"
#include "gamecontext.h"
#include "graphics.h"
#include "image_buffer.h"
#include "logging.h"
#include "map.h"
#include "networking.h"

#include "ncurses.h"

#include "fcntl.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "strings.h"
#include "time.h"

/* client includes */

#include <netdb.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include "unistd.h"
#define SA struct sockaddr 
#define MAX_MSG_SIZE 4096
#define PORT 8080 
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

int local_create_entity_handler(Event_t ev, GameContext_t* ctx) {
  entity_id id = Entity_create(ctx->entity_pool);
  Entity_t *e = Entity_get(ctx->entity_pool, id);
  if (ctx->player == NULL) {
    ctx->player = e;
  }
  Graphics_draw_line(e->image.frames[0], 7, 8, 7, 17, ImageBuffer_COLOR_MAGENTA);
  Graphics_draw_line(e->image.frames[0], 8, 8, 8, 17, ImageBuffer_COLOR_RED);
  Graphics_draw_line(e->image.frames[0], 6, 8, 6, 17, ImageBuffer_COLOR_YELLOW);
  Graphics_draw_line(e->image.frames[0], 5, 8, 5, 17, ImageBuffer_COLOR_GREEN);
  Graphics_draw_line(e->image.frames[0], 4, 8, 4, 17, ImageBuffer_COLOR_CYAN);
  return 0;
}

typedef enum {
  LOCAL_MODE,
  CLIENT_MODE,
  SERVER_MODE
} runmode;
  
char msg_buff[MAX_MSG_SIZE]; 
int main(int argc, char** argv) {
  runmode mode = LOCAL_MODE;
  if (argc > 1) {
    if (strncmp(argv[1], "server", 6) >= 0) {
      mode = SERVER_MODE; 
    } else if (strncmp(argv[1], "client", 6) >= 0) {
      mode = CLIENT_MODE; 
    }
  }


  /* SERVER MODE */
  if (mode == SERVER_MODE) {
    
    // NEVER ENDING MOVE COMMANDS FOR TESTING 
    Event_t ev;
    EntityMoveEvent move_right_event = {.id=1, .delta_x=1, .delta_y=0};
    ev.type = ENTITY_MOVE;
    ev.data = (event)move_right_event;
    Message_t msg = Networking_new_message(EVENT, &ev, sizeof(ev));

    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
 
    int flag = 1;  
    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {  
      perror("setsockopt fail");  
    }  

    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
  
    struct timespec waittime = {.tv_sec = 1, .tv_nsec = 999999999 / FPS};
    for (;;) { 
        ssize_t n_bytes = write(connfd, &msg, sizeof(msg));
        printf("bytes written: %lu\n", n_bytes);
        nanosleep(&waittime, NULL);
    } 
  
    close(sockfd); 
    return 0;
  }

  // Outside of server mode we'll need a graphical interface.

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

  // Setup game context
  GameContext_t ctx = {.map=map_ptr, .entity_pool=entity_pool_ptr, .player=NULL};

  // Setup camera.
  Camera_t *camera_ptr = (Camera_t*)(malloc(sizeof(Camera_t)));
  camera_ptr->x = 0;
  camera_ptr->y = 0;

  // Initialize event handlers
  EventBus_t event_bus;
  event_bus.queue_top = 0;
  EventBus_register_handler(&event_bus, ENTITY_CREATE, local_create_entity_handler);
  EventBus_register_handler(&event_bus, ENTITY_MOVE, local_movement_handler);

  // start and stop the game loop.
  bool running = true;
  /* CLIENT MODE */
  if (mode == CLIENT_MODE) {
  
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  Event_t ev;
  EntityCreateEvent create_entity_event = {};
  ev.type = ENTITY_CREATE;
  ev.data = (event)create_entity_event;
  EventBus_push(&event_bus, ev);
  EventBus_push(&event_bus, ev);
  EventBus_handle_events(&event_bus, &ctx);
  Entity_t *player = ctx.player;
  if (player == NULL) {
    return -99;
  }

  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

  // Networking buffer.
  while (running) {
    // Dynamic screen resizing.
    int check_max_x, check_max_y = 0;
    getmaxyx(stdscr, check_max_y, check_max_x);
    if (check_max_x != max_x || check_max_y != max_y)
      screen_ptr = ImageBuffer_new(check_max_x, check_max_y);

    // Input handling.
    EntityMoveEvent move_right_event = {.id=player->id, .delta_x=1, .delta_y=0};
    EntityMoveEvent move_left_event = {.id=player->id, .delta_x=-1, .delta_y=0};
    EntityMoveEvent move_up_event = {.id=player->id, .delta_x=0, .delta_y=1};
    EntityMoveEvent move_down_event = {.id=player->id, .delta_x=0, .delta_y=-1};
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

    // Check for network messages. 
    int bytes_in = read(sockfd, msg_buff, sizeof(msg_buff));
    if (bytes_in > 0) {
      Message_t *msg = (Message_t*)msg_buff;
      if (msg->type == EVENT) {
        Event_t network_ev;
        memcpy(&network_ev, msg->payload, sizeof(Event_t));
        EventBus_push(&event_bus, network_ev);
      }
    }

    // Event processing.
    EventBus_handle_events(&event_bus, &ctx);

    // Rendering.
    Camera_follow(camera_ptr, player);
    Camera_draw(camera_ptr, map_ptr, entity_pool_ptr, screen_ptr);
    Graphics_blit(screen_ptr);
    refresh();
    nanosleep(&waittime, NULL);
		ImageBuffer_clear(screen_ptr);
  }
  
    // Close the socket. 
    close(sockfd); 
    return 0;
  }

}
