#include "networking.h"

Message_t Networking_serialize_event(Event_t ev) {
  Message_t msg;
  msg.size = sizeof(msg) + sizeof(ev);
  msg.type = EVENT;
  memcpy(&msg.payload, &ev, sizeof(ev));
  return msg;
}
