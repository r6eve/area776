#include "def_global.hpp"
#include "image_manager.hpp"
#include "bg.hpp"

static Snow_data Snow[NUM_SNOW];

void init_bg() {
  for (int i = 0; i < NUM_SNOW; ++i) {
    Snow[i].view = false;
  }
}

void update_bg() {
  for (int i = 0; i < NUM_SNOW; ++i) {
    if (Snow[i].view) {
      continue;
    }
    Snow[i].view = true;
    Snow[i].type = rand() % 2;
    Snow[i].x = rand() % (SCREEN_WIDTH - 8);
    Snow[i].y = -8;
    break;
  }
  for (int i = 0; i < NUM_SNOW; ++i) {
    if (!Snow[i].view) {
      continue;
    }
    int move_speed[2] = {12, 8};
    Snow[i].y += move_speed[Snow[i].type];
    if (Snow[i].y >= SCREEN_HEIGHT) {
      Snow[i].view = false;
    }
  }
}

void draw_bg() {
  for (int i = 0; i < NUM_SNOW; ++i) {
    if (!Snow[i].view) {
      continue;
    }
    SDL_Surface *p_surface = get_img("snow");
    SDL_Rect src = {8 * Snow[i].type, 0, 8, 8};
    SDL_Rect dst = {Snow[i].x, Snow[i].y};
    SDL_BlitSurface(p_surface, &src, Screen, &dst);
  }
}
