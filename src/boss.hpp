#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

struct Boss {
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

  /**
   * Return true if boss life is 0.
   */
  bool check_myshots_hit_boss();

  void draw(SDL_Surface *screen, ImageManager &image_manager);
  void draw_shot(SDL_Surface *screen, ImageManager &image_manager);

  ~Boss() {}
};

#endif
