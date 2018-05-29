#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
#include "effect.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

struct Boss {
  enum class boss_state {
    automove,
    attack00,
    attack01,
    attack02,
  };

  struct Bullet {
    bool view;
    int rot;
    int count;
    Point pos;
    Point move;
  };

  boss_state state;
  int x;
  int y;
  int move;
  int shot_rot;
  int shot_count;
  int life;
  Bullet bullets[BOSS_SHOT_MAX];

  Boss() {}

  void init();
  void update(MixerManager &mixer_manager);
  void update_shot();
  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);

  ~Boss() {}
};

#endif
