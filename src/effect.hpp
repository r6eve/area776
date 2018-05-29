#ifndef EFFECT_H
#define EFFECT_H

#include "def_global.hpp"
#include "image_manager.hpp"

struct Effect {
  struct EffectData {
    bool view;
    Point pos;
    int count;
  };

  EffectData effects[EFFECT_MAX];

  inline void init() {
    for (auto &effect : effects) {
      effect.view = false;
    }
  }

  inline void update() {
    for (auto &effect : effects) {
      if (!effect.view) {
        continue;
      }
      ++effect.count;
      if (effect.count >= 24) {
        effect.view = false;
      }
    }
  }

  inline void draw(SDL_Surface *screen, ImageManager &image_manager) {
    for (auto &effect : effects) {
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
};

#endif
