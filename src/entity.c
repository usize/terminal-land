#include "entity.h"

static Entity_t *entities[MAX_ENTITIES];

entity_id Entity_create() {
  static int nonce = 0;
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (entities[i] == NULL || entities[i]->destroyed) {
      Entity_t *e = entities[i];
      // Try to reuse existing entities
      if (e == NULL) {
        e = (Entity_t*)_allocate(sizeof(Entity_t));
        for (int i = 0; i < MAX_IMAGE_FRAMES; i++) {
          e->image.frames[i] = ImageBuffer_new(MAX_IMAGE_SIZE, MAX_IMAGE_SIZE); 
        } 
      } else {
        for (int i = 0; i < MAX_IMAGE_FRAMES; i++) {
          ImageBuffer_clear(e->image.frames[i]);
        } 
      }
      while ((nonce % MAX_ENTITIES) != i) nonce++;
      e->id = nonce;
      e->destroyed = false;
      e->position.x = 0;
      e->position.y = 0;
      e->physics.y_acc = -10.0;
      e->physics.x_acc = 0.0;
      e->image.framecount = 0;
      entities[i] = e;
      nonce++;
      return e->id;
    } 
  }
  return -1;
}

Entity_t* Entity_get(entity_id id) {
  return entities[id % MAX_ENTITIES];
}

void Entity_destroy(entity_id id) {
  Entity_t *e = Entity_get(id % MAX_ENTITIES);
  if (e) e->destroyed = true;
}

static int entities_iterator = 0;
Entity_t* Entity_iterator() {
  Entity_t *e = NULL;
  while (e == NULL && entities_iterator < MAX_ENTITIES) {
    e = Entity_get(entities_iterator);
    entities_iterator++;
  }
  return e;
}

void Entity_clear_iterator() {
  entities_iterator = 0;
}
