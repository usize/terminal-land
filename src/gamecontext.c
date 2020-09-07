#include "gamecontext.h"

GameContext_t* GameContext_new(Map_t *m, EntityPool_t *ep) {
  GameContext_t *ctx = (GameContext_t*)malloc(sizeof(*ctx));
  ctx->map = m;
  ctx->entity_pool = ep;
  ctx->player = NULL;
  return ctx;
}
