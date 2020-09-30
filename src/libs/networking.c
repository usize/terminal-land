#include "networking.h"

// This implementation is terrible because it doesn't account for
// differences between architectures or operating systems. These
// include: endianness, structure packing, and type sizes.
Message_t Networking_new_message(message_payload_type type, 
                                   void* payload,
                                   size_t payload_size) {
  Message_t msg;
  msg.type = type;
  memcpy(&msg.payload, payload, payload_size);
  return msg;
}
