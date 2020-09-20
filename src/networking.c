#include "networking.h"

Message_t Networking_new_message(message_payload_type type, 
                                   void* payload,
                                   size_t payload_size) {
  Message_t msg;
  msg.type = type;
  memcpy(&msg.payload, payload, payload_size);
  return msg;
}
