#ifndef ENEMY_H
#define ENEMY_H

#include "area776.hpp"
#include "vector.hpp"

struct Enemy_data {
  bool view;
  Vector pos;
  Vector move;
  int shot_timer;
};

void init_enemy();
void appear_enemy();
void mv_enemy();
void mv_enemy_shot();
void check_myshots_hit_enemy();
void draw_enemy();
void draw_enemy_shot();

#endif
