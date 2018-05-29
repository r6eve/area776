#ifndef FIGHTER_H
#define FIGHTER_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"

struct Enemy;

struct Fighter {
  struct Bullet {
    bool view;
    Point pos;
    Point move;
  };

  Point pos;
  int shot_timer;
  int life;
  Bullet bullets[FIGHTER_SHOT_MAX];

  Fighter() {}

  void init();
  void update(InputManager &input_manager, MixerManager &mixer_manager);
  void update_shot();

  /**
   * Return true if player life is 0.
   */
  bool check_enemyshots_hit_mychara(enemy_type enemy_select, Enemy &enemy,
                                    Boss &boss, Effect &effect);

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);

  ~Fighter() {}
};

#endif
