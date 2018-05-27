#ifndef FIGHTER_H
#define FIGHTER_H

#include "vector.hpp"
#include "input_manager.hpp"

struct Fighter_data {
  Vector pos;
  int shot_timer;
};

void init_fighter();
void move_fighter(InputManager &input_manager);
void move_fighter_shot();
void check_enemyshots_hit_mychara();
void draw_fighter();
void draw_fighter_shot();

#endif
