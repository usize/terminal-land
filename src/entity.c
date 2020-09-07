#include "entity.h"

entity_id Entity_create(EntityPool_t *ep) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity_t *e = &ep->entities[i];
    if (e->id < 0 || e->destroyed) {
      // Always give recycled destroyed entities a new id. 
      while ((ep->nonce % MAX_ENTITIES) != i) ep->nonce++;
      e->id = ep->nonce;
      e->destroyed = false;
      e->position.x = 0;
      e->position.y = MAX_IMAGE_SIZE >> 1;
      e->image.framecount = 0;
      ep->nonce++;
      for (int j = 0; j < MAX_IMAGE_FRAMES; j++) {
        if (e->image.frames[j] != NULL) {
          ImageBuffer_clear(e->image.frames[j]);
        } else {
          e->image.frames[j] = ImageBuffer_new(MAX_IMAGE_SIZE, MAX_IMAGE_SIZE);
        } 
      }
      return e->id;
    } 
  }
  return -1;
}

Entity_t* Entity_get(EntityPool_t *ep, entity_id id) {
  Entity_t *e = &ep->entities[id % MAX_ENTITIES];
  if (e->id < 0 || e->destroyed) {
    return NULL;
  }
  return e;
}

void Entity_destroy(EntityPool_t *ep, entity_id id) {
  Entity_t *e = Entity_get(ep, id % MAX_ENTITIES);
  e->destroyed = true;
}

Entity_t* EntityPool_iterator(EntityPool_t *ep) {
  Entity_t *e = NULL;
  while (e == NULL && ep->entities_iterator < MAX_ENTITIES) {
    e = Entity_get(ep, ep->entities_iterator); 
    ep->entities_iterator++;
  }
  return e;
}

void EntityPool_clear_iterator(EntityPool_t *ep) {
  ep->entities_iterator = 0;
}

EntityPool_t* EntityPool_create() {
  EntityPool_t *ep = malloc(sizeof(*ep));
  ep->nonce = 0;
  ep->entities_iterator = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    ep->entities[i].id = -1;
  }
  return ep;
}

void EntityPool_destroy(EntityPool_t *ep) {
  EntityPool_clear_iterator(ep);
  for (Entity_t *e = EntityPool_iterator(ep); e != NULL; e = EntityPool_iterator(ep)) {
    for (int j = 0; j < MAX_IMAGE_FRAMES; j++) {
      if (e->image.frames[j] != NULL) {
        free(e->image.frames[j]);
      }
    }
  }
  free(ep);
}
