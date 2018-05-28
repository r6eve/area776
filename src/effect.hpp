#ifndef EFFECT_H
#define EFFECT_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "point.hpp"

struct Effect_data {
  bool view;
  Point pos;
  int count;
};

void init_effect();
void update_effect();
void draw_effect(SDL_Surface *screen, ImageManager &image_manager);

#endif
