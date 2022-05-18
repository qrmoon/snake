#include <stdlib.h>
#include <stdbool.h>
#include "vec2.h"

typedef struct {
  vec2i* body;
  size_t size;
  vec2i dir;
  vec2i last_dir;
} snake_t;

snake_t snake_init(int, int, int);
bool snake_move(snake_t*);
int snake_overlaps(snake_t*, vec2i);
bool snake_grow(snake_t* snk);
#ifdef SNAKE_USING_SDL
void snake_draw(snake_t*, SDL_Surface*);
#endif


snake_t snake_init(int x, int y, int size) {
  snake_t* snk = malloc(sizeof(snake_t));
  snk->body = malloc(sizeof(vec2i) * size);
  snk->size = size;
  snk->dir.x = 0;
  snk->dir.y = -1;
  snk->last_dir = snk->dir;

  for (int i = 0; i < size; i++) {
    snk->body[i].x = x;
    snk->body[i].y = y+i;
  }

  return *snk;
}

bool snake_move(snake_t* snk) {
  for (int i = snk->size-1; i > 0; i--) {
    snk->body[i] = snk->body[i-1];
  }

  vec2i* pos = &snk->body[0];

  pos->x += snk->dir.x;
  pos->y += snk->dir.y;

#ifdef SNAKE_LIMIT_X
#ifdef SNAKE_LIMIT_Y

#ifdef SNAKE_WRAP
  if (pos->x > SNAKE_LIMIT_X) pos->x = 0;
  else if (pos->x < 0) pos->x = SNAKE_LIMIT_X;
  else if (pos->y > SNAKE_LIMIT_Y) pos->y = 0;
  else if (pos->y < 0) pos->y = SNAKE_LIMIT_Y;
#else
  if (pos->x > SNAKE_LIMIT_X || pos->x < 0 || pos->y > SNAKE_LIMIT_Y || pos->y < 0) {
    return false;
  }
#endif

#endif
#endif


  for (int i = 1; i < snk->size; i++) {
    if (veceq(*pos, snk->body[i])) {
      return false;
    }
  }

  return true;
}

int snake_overlaps(snake_t* snk, vec2i o) {
  for (int i = 0; i < snk->size; i++) {
    if (veceq(snk->body[i], o)) return i;
  }

  return -1;
}

bool snake_grow(snake_t* snk) {
  vec2i* body = realloc(snk->body, sizeof(vec2i) * (snk->size+1));

  if (body == NULL) {
    return false;
  }

  snk->body = body;
  vec2i* prevLast = &snk->body[snk->size-1];
  snk->body[snk->size].x = prevLast->x;
  snk->body[snk->size].y = prevLast->y;
  snk->size++;
  return true;
}

#ifdef SNAKE_USING_SDL
void snake_draw(snake_t* snk, SDL_Surface* surface) {
  SDL_Rect rect = {0, 0, SNAKE_GRID_SIZE, SNAKE_GRID_SIZE};
  for (int i = snk->size-1; i >= 0; i--) {
    rect.x = snk->body[i].x * SNAKE_GRID_SIZE;
    rect.y = snk->body[i].y * SNAKE_GRID_SIZE;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));
  }
}
#endif
