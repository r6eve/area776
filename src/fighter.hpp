#ifndef FIGHTER_H
#define FIGHTER_H

#include "boss.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "point.hpp"

class FighterClass {
 public:
  void init();
  void move(InputManager &input_manager, MixerManager &mixer_manager);
  void move_shot();

  /**
   * Return true if player life is 0.
   */
  bool check_enemyshots_hit_mychara(Boss &boss);

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);
};

#endif
