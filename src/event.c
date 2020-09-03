#include "event.h"

void Event_router(Event_t *ev) {
  switch (ev->_type) {
    case ENTITY_CREATE:
      break;
    case ENTITY_DESTROY:
      break;
    case ENTITY_POSITION_CHANGE:
      break;
  }
}
