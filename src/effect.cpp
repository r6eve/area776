#include "def_global.hpp"
#include "image_manager.hpp"

void init_effect() {
  for (int i = 0; i < EFFECT_MAX; ++i) {
    Effect[i].view = false;
  }
}

void update_effect() {
  for (int i = 0; i < EFFECT_MAX; ++i) {
    if (!Effect[i].view) {
      continue;
    }
    ++Effect[i].count;
    if (Effect[i].count >= 24) {
      Effect[i].view = false;
    }
  }
}

void draw_effect(ImageManager &image_manager) {
  for (int i = 0; i < EFFECT_MAX; ++i) {
    if (!Effect[i].view) {
      continue;
    }
    int n = Effect[i].count / 2;
    SDL_Surface *p_surface = image_manager.get(image::effect01);
    SDL_Rect src = {n % 4 * 160, n / 4 * 160, 160, 160};
    SDL_Rect dst = {Effect[i].pos.x, Effect[i].pos.y};
    SDL_BlitSurface(p_surface, &src, Screen, &dst);
  }
}
