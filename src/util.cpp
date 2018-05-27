#include "util.hpp"

bool check_hit_rect(SDL_Rect *a, SDL_Rect *b) {
  if (((a->x) < (b->x + b->w)) && ((b->x) < (a->x + a->w)) &&
      ((a->y) < (b->y + b->h)) && ((b->y) < (a->y + a->h))) {
    return true;
  } else {
    return false;
  }
}
