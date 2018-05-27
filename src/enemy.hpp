#ifndef ENEMY_H
#define ENEMY_H

#include "area776.hpp"
#include "vector.hpp"
#include "mixer_manager.hpp"

struct Enemy_data {
  bool view;
  Vector pos;
  Vector move;
  int shot_timer;
};

void init_enemy();
void appear_enemy();
void move_enemy(MixerManager &mixer_manager);
void move_enemy_shot();
void check_myshots_hit_enemy();
void draw_enemy(SDL_Surface *screen, ImageManager &image_manager);
void draw_enemy_shot(SDL_Surface *screen, ImageManager &image_manager);

#endif
