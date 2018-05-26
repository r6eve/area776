#ifndef BOSS_H
#define BOSS_H

#include "vector.hpp"

struct Boss_data {
  int state;
  int x;
  int y;
  int move;
  int shot_rot;
  int shot_count;
};

struct Boss_shot_data {
  bool view;
  int rot;
  int count;
  Vector pos;
  Vector move;
};

void init_boss();
void mv_boss();
void mv_boss_shot();
void check_myshots_hit_boss();
void draw_boss();
void draw_boss_shot();

#endif
