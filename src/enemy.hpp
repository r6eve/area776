#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "fighter.hpp"
#include "effect.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

struct Enemy {
  struct EnemyData {
    bool view;
    Point pos;
    Point move;
    int shot_timer;
  };

  struct Bullet {
    bool view;
    Point pos;
    Point move;
  };

  int life;
  EnemyData enemies[ENEMY_MAX];
  Bullet bullets[ENEMY_SHOT_MAX];

  Enemy() {}

  void init();
  void appear(Fighter &fighter);
  void update(MixerManager &mixer_manager, Fighter &fighter);
  void update_shot();

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);

  ~Enemy() {}
};

#endif
