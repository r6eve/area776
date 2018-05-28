#ifndef FIGHTER_H
#define FIGHTER_H

#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "vector.hpp"

struct Fighter_data {
  Vector pos;
  int shot_timer;
};

void init_fighter();
void move_fighter(InputManager &input_manager, MixerManager &mixer_manager);
void move_fighter_shot();

/**
 * Return true if player life is 0.
 */
bool check_enemyshots_hit_mychara();

void draw_fighter(SDL_Surface *screen, ImageManager &image_manager);
void draw_fighter_shot(SDL_Surface *screen, ImageManager &image_manager);

#endif
