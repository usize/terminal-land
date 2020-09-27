#include "entity.h"

entity_id Entity_create(EntityPool_t *ep) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    Entity_t *e = &ep->entities[i];
    if (e->id < 0 || e->destroyed) {
      // Always give recycled destroyed entities a new id. 
      while ((ep->nonce % MAX_ENTITIES) != i) ep->nonce++;
      e->player = false;
      e->id = ep->nonce;
      e->destroyed = false;
      e->position.x = 0;
      e->position.y = MAX_IMAGE_SIZE >> 1;
      e->image.framecount = 0;
      e->image.current_frame = 0;
      e->image.ids[0] = 0;
      ep->nonce++;
      return e->id;
    } 
  }
  return -1;
}

void EntityPool_push(EntityPool_t *ep, const Entity_t e) {
  entity_id id = e.id;
  memcpy(&ep->entities[id % MAX_ENTITIES], &e, sizeof(e));
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
  ImageBuffer_t* default_img = ImageBuffer_new(MAX_IMAGE_SIZE, MAX_IMAGE_SIZE);
  Graphics_draw_line(default_img, 7, 8, 7, 17, ImageBuffer_MAGENTA);
  Graphics_draw_line(default_img, 8, 8, 8, 17, ImageBuffer_RED);
  Graphics_draw_line(default_img, 6, 8, 6, 17, ImageBuffer_YELLOW);
  Graphics_draw_line(default_img, 5, 8, 5, 17, ImageBuffer_GREEN);
  Graphics_draw_line(default_img, 4, 8, 4, 17, ImageBuffer_CYAN);
  ep->images[0] = default_img; 
  return ep;
}

void EntityPool_destroy(EntityPool_t *ep) {
  for (int i = 0; i < MAX_IMAGES; i++) {
    if (ep->images[i] != NULL) free(ep->images[i]); 
  }
  free(ep);
}
