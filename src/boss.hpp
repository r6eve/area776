#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

class BossClass {
 public:
  void init();
  void move(MixerManager &mixer_manager);
  void move_shot();

  /**
   * Return true if boss life is 0.
   */
  bool check_myshots_hit_boss();

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);
};

#endif
