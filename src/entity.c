#include "entity.h"

static Entity_t *entities[MAX_ENTITIES];
static int entities_tail = 0;
static int entities_iterator = 0;

entity_id Entity_create() {
  Entity_t *e = (Entity_t*)_allocate(sizeof(Entity_t));
  e->destroyed = false;
  e->position.x = 0;
  e->position.y = 0;
  e->physics.y_acc = -10.0;
  e->physics.x_acc = 0.0;
  e->image.framecount = 0;
  // Go ahead and allocate all of the image data up front.
  for (int i = 0; i < MAX_IMAGE_FRAMES; i++) {
    e->image.frames[i] = ImageBuffer_new(MAX_IMAGE_SIZE, MAX_IMAGE_SIZE); 
  } 

  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (entities[i] == NULL) {
      entities[i] = e;
      if (entities_tail < i) {
        entities_tail = i;
      }
      return i;
    } 
  }
  return -1;
}

Entity_t* Entity_get(entity_id id) {
  return entities[id];
}

void Entity_destroy(entity_id id) {
  Entity_t *e = Entity_get(id);
  if (e) e->destroyed = true;
}

Entity_t* Entity_iterator() {
  Entity_t *e = NULL;
  while (e == NULL 
         && entities_iterator < entities_tail 
         && entities_iterator < MAX_ENTITIES) {
    e = Entity_get(entities_iterator);
    entities_iterator++;
  }
  return e;
}

void Entity_clear_iterator() {
  entities_iterator = 0;
}
