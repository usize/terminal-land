#pragma once

#include "event.h"
#include "string.h"
#include "strings.h"

#define MAX_PAYLOAD_SIZE 512

typedef enum {
  EVENT,
  ENTITY,
} message_payload_type;

typedef struct {
  unsigned int size;
  message_payload_type type;
  char payload[MAX_PAYLOAD_SIZE];
} Message_t;

Message_t Networking_serialize_event(Event_t);
