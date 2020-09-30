#pragma once

#include "event.h"
#include "string.h"
#include "strings.h"

#define MAX_PAYLOAD_SIZE 512

typedef enum {
  MSG_EVENT,
  MSG_ENTITY,
} message_payload_type;

typedef struct {
  message_payload_type type;
  char payload[MAX_PAYLOAD_SIZE];
} Message_t;

Message_t Networking_new_message(message_payload_type, void*, size_t);
