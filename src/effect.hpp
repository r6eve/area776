#ifndef EFFECT_H
#define EFFECT_H

#include "vector.hpp"

struct Effect_data {
  bool view;
  Vector pos;
  int count;
};

void init_effect();
void update_effect();
void draw_effect();

#endif
