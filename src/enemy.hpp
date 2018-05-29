#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "point.hpp"

class EnemyClass {
 public:
  void init();
  void appear();
  void move(MixerManager &mixer_manager);
  void move_shot();

  /**
   * Return true if enemy life is 0.
   */
  bool check_myshots_hit_enemy();

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);
};

#endif
