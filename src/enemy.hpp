#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "mixer_manager.hpp"
#include "point.hpp"

struct Enemy_data {
  bool view;
  Point pos;
  Point move;
  int shot_timer;
};

void init_enemy();
void appear_enemy();
void move_enemy(MixerManager &mixer_manager);
void move_enemy_shot();

/**
 * Return true if enemy life is 0.
 */
bool check_myshots_hit_enemy();

void draw_enemy(SDL_Surface *screen, ImageManager &image_manager);
void draw_enemy_shot(SDL_Surface *screen, ImageManager &image_manager);

#endif
