#include "def_global.hpp"
#include "image_manager.hpp"
#include "point.hpp"

void init_effect() {
  for (auto &effect : Effect) {
    effect.view = false;
  }
}

void update_effect() {
  for (auto &effect : Effect) {
    if (!effect.view) {
      continue;
    }
    ++effect.count;
    if (effect.count >= 24) {
      effect.view = false;
    }
  }
}

void draw_effect(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &effect : Effect) {
    if (!effect.view) {
      continue;
    }
    const int n = effect.count / 2;
    SDL_Surface *p_surface = image_manager.get(image::effect01);
    SDL_Rect src = {static_cast<Sint16>(n % 4 * 160),
                    static_cast<Sint16>(n / 4 * 160), 160, 160};
    SDL_Rect dst = {static_cast<Sint16>(effect.pos.x),
                    static_cast<Sint16>(effect.pos.y), 160, 160};
    SDL_BlitSurface(p_surface, &src, screen, &dst);
  }
}
