#include "event.h"

int EventBus_push(EventBus_t* eb, Event_t e) {
 if (eb->queue_top > MAX_EVENTS - 1) {
   return -1;
 }
 eb->queue[eb->queue_top] = e;
 eb->queue_top++;
 return eb->queue_top;
}

Event_t EventBus_pop(EventBus_t* eb) {
  if (eb->queue_top <= 0) {
    Event_t e = {.type=NULL_EVENT};
    return e;
  }
  eb->queue_top--;
  Event_t e = eb->queue[eb->queue_top];
  return e;
}

void EventBus_register_handler(EventBus_t* eb, event_type et, event_handler eh) {
  eb->handlers[et] = eh;
}

void EventBus_handle_events(EventBus_t* eb, GameContext_t *ctx) {
  while (eb->queue_top > 0) {
    Event_t e = EventBus_pop(eb);
    event_handler eh = eb->handlers[e.type];
    if (eh != NULL) {
      eh(e, ctx); 
    }
  } 
}
