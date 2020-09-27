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

#include "errno.h"
#include "fcntl.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "strings.h"
#include "time.h"

/* client includes */

#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include "unistd.h"
#define SA struct sockaddr 
#define MAX_CLIENTS 256
#define SERVER_PORT 8080 
#define FPS 60


int local_movement_handler(Event_t ev, GameContext_t* ctx) {
  EntityMoveEvent *move_event = (EntityMoveEvent*)&ev.data;
  entity_id id = move_event->id;
  Entity_t *e = Entity_get(ctx->entity_pool, id);
  if (e == NULL) {
    return -1;
  }
  e->position.x = move_event->to_x;
  e->position.y = move_event->to_y;
  return 0;
}

int local_create_entity_handler(Event_t ev, GameContext_t* ctx) {
  entity_id id = Entity_create(ctx->entity_pool);
  Entity_t *e = Entity_get(ctx->entity_pool, id);
  if (ctx->player == NULL) {
    ctx->player = e;
  }
  Graphics_draw_line(e->image.frames[0], 7, 8, 7, 17, ImageBuffer_MAGENTA);
  Graphics_draw_line(e->image.frames[0], 8, 8, 8, 17, ImageBuffer_RED);
  Graphics_draw_line(e->image.frames[0], 6, 8, 6, 17, ImageBuffer_YELLOW);
  Graphics_draw_line(e->image.frames[0], 5, 8, 5, 17, ImageBuffer_GREEN);
  Graphics_draw_line(e->image.frames[0], 4, 8, 4, 17, ImageBuffer_CYAN);
  return 0;
}

typedef enum {
  NO_MODE,
  CLIENT_MODE,
  SERVER_MODE
} runmode;
  
char msg_buf[sizeof(Message_t) * 2]; 
int main(int argc, char** argv) {
  runmode mode = NO_MODE;
  if (argc > 1) {
    if (strncmp(argv[1], "server", 6) >= 0) {
      mode = SERVER_MODE; 
    } else if (strncmp(argv[1], "client", 6) >= 0) {
      mode = CLIENT_MODE; 
    } else {
      printf("usage: %s <server|client>", argv[0]);
      exit(0);
    }
  }


  /* SERVER MODE */
  if (mode == SERVER_MODE) {
    
    int serverfd;
    int clientfds[MAX_CLIENTS] = {0};
    struct sockaddr_in server_addr, client_addr; 
  
    // socket create and verification 
    serverfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverfd == -1) {
        perror("socket creation failed....");
        exit(1); 
    } 
    
    // assign IP, SERVER_PORT 
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_addr.sin_port = htons(SERVER_PORT); 
 
    int flag = 1;  
    if (-1 == setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {  
      perror("setsockopt fail");
      exit(1);
    }  

    // Binding newly created socket to given IP and verification 
    if ((bind(serverfd, (SA*)&server_addr, sizeof(server_addr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(1); 
    } 
  
    // Now server is ready to listen and verification 
    if ((listen(serverfd, 1)) < 0) { 
        printf("Listen failed...\n"); 
        exit(1); 
    } 

    fd_set selectfds;
    int max_fd = serverfd;
    for (;;) {
      FD_ZERO(&selectfds);
      FD_SET(serverfd, &selectfds);
      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientfds[i] > 0) {
          int fd = clientfds[i];
          FD_SET(fd, &selectfds);
          if (fd > max_fd) max_fd = fd;
        }
      }

      int ready = select(max_fd + 1, &selectfds, NULL, NULL, NULL);
      if ((ready < 0) && (errno != EINTR)) {
        perror("select error"); 
      }

      // Handle new connections.
      if (FD_ISSET(serverfd, &selectfds)) {
        socklen_t len;
        int new_clientfd = accept(serverfd, (SA*)&client_addr, &len);
        if (new_clientfd < 0) {
          perror("accept client"); 
        }
        
        for (int i = 0; i < MAX_CLIENTS; i++) {
          if (clientfds[i] < 1) {
            clientfds[i] = new_clientfd;
            break; 
          }
        }
      }

      // Handle clients sending data
      for (int i = 0; i < MAX_CLIENTS; i++) {
        int clientfd = clientfds[i];
        if (FD_ISSET(clientfd, &selectfds)) {
          int read_len;
          if ((read_len = read(clientfd, msg_buf, sizeof(msg_buf))) == 0) {
            // Client is quitting.
            close(clientfd);
            clientfds[i] = 0; 
          }
          // Send the message to all the other clients
          for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = clientfds[i];
            if (fd > 0 && fd != clientfd) {
              write(fd, msg_buf, sizeof(msg_buf)); 
            }
          }
        } 
      }

    }

    close(serverfd); 
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

  init_pair(ImageBuffer_BLACK, COLOR_BLACK, COLOR_BLACK);
  init_pair(ImageBuffer_WHITE, COLOR_WHITE, COLOR_WHITE);
  init_pair(ImageBuffer_RED, COLOR_RED, COLOR_RED);
  init_pair(ImageBuffer_GREEN, COLOR_GREEN, COLOR_GREEN);        
  init_pair(ImageBuffer_BLUE, COLOR_BLUE, COLOR_BLUE);        
  init_pair(ImageBuffer_CYAN, COLOR_CYAN, COLOR_CYAN);
  init_pair(ImageBuffer_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(ImageBuffer_YELLOW, COLOR_YELLOW, COLOR_YELLOW);

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
  
    int serverfd; 
    struct sockaddr_in server_addr; 
  
    // socket create and varification 
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
  
    // assign IP, SERVER_PORT 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_addr.sin_port = htons(SERVER_PORT); 
  
    // connect the client socket to server socket 
    if (connect(serverfd, (SA*)&server_addr, sizeof(server_addr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    fcntl(serverfd, F_SETFL, O_NONBLOCK);

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

    // A single message struct for sending messages to the server.
    Message_t msg_out = {0};
    msg_out.type = MSG_EVENT;

    // Networking buffer.
    while (running) {
      // Dynamic screen resizing.
      int check_max_x, check_max_y = 0;
      getmaxyx(stdscr, check_max_y, check_max_x);
      if (check_max_x != max_x || check_max_y != max_y)
        screen_ptr = ImageBuffer_new(check_max_x, check_max_y);

      // Input handling.
      char input_c = getch(); 
      
      ev.type = ENTITY_MOVE;
      if (input_c == 'd') {
        EntityMoveEvent move_right_event = {
          .id=player->id, 
          .from_x=player->position.x, 
          .from_y=player->position.y,
          .to_x=player->position.x + 1, 
          .to_y=player->position.y,
        };
        ev.data = (event)move_right_event;
        memcpy(msg_out.payload, (char*)&ev, sizeof(ev));
        write(serverfd, (char*)&msg_out, sizeof(msg_out));
        EventBus_push(&event_bus, ev);
      }

      // Check for network messages. 
      int bytes_in = read(serverfd, msg_buf, sizeof(msg_buf));
      if (bytes_in > 0) {
        Message_t *msg = (Message_t*)msg_buf;
        if (msg->type == MSG_EVENT) {
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
    close(serverfd); 
    return 0;
  }
}
