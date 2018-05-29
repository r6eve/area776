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

  void init();
  void update();
  void draw(SDL_Surface *screen, ImageManager &image_manager);
};

#endif
