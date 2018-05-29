#ifndef EFFECT_H
#define EFFECT_H

#include "def_global.hpp"
#include "image_manager.hpp"

class EffectClass {
 public:
  void init();
  void update();
  void draw(SDL_Surface *screen, ImageManager &image_manager);
};

#endif
