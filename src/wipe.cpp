#include "def_global.hpp"

static int Wipe_variation;
static int Wipe_count;
static int Wipe_count_reach;

void set_wipe_in() {
  Wipe_variation = -1;
  Wipe_count = SCREEN_HEIGHT / 10;
  Wipe_count_reach = 0;
}

void set_wipe_out() {
  Wipe_variation = 1;
  Wipe_count = 0;
  Wipe_count_reach = SCREEN_HEIGHT / 10;
}

bool update_wipe() {
  if (Wipe_count != Wipe_count_reach) {
    Wipe_count += Wipe_variation;
    return false;
  } else {
    return true;
  }
}

void draw_wipe(int start_y) {
  SDL_Rect dst;
  Uint32 black = 0x00000000;
  dst.x = 0;
  dst.w = SCREEN_WIDTH;
  dst.h = Wipe_count;
  for (dst.y = start_y; dst.y < SCREEN_HEIGHT; dst.y += 48) {
    SDL_FillRect(Screen, &dst, black);
  }
}
