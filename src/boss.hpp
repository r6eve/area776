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

  inline void init() {
    state = boss_state::automove;
    x = (screen::width - 418) / 2;
    y = -240;
    move = 0;
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
  }

  void update(MixerManager &mixer_manager);

  inline void update_shot() {
    for (auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }
      bullet.pos.add(bullet.move);
      if (bullet.pos.x < -16) {
        bullet.view = false;
      }
      if (bullet.pos.y < -16) {
        bullet.view = false;
      }
      if (bullet.pos.x > screen::width) {
        bullet.view = false;
      }
      if (bullet.pos.y > screen::height) {
        bullet.view = false;
      }
      ++bullet.count;
      bullet.count %= 2;
      if (bullet.count == 0) {
        ++bullet.rot;
        bullet.rot %= 16;
      }
    }
  }

  inline void draw(SDL_Surface *screen, ImageManager &image_manager) {
    SDL_Surface *p_surface = image_manager.get(image::boss);
    SDL_Rect dst = {static_cast<Sint16>(x), static_cast<Sint16>(y), 400, 224};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }

  inline void draw_shot(SDL_Surface *screen, ImageManager &image_manager) {
    for (auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }
      SDL_Surface *p_surface = image_manager.get(image::bm01);
      SDL_Rect dst = {static_cast<Sint16>(bullet.pos.x),
                      static_cast<Sint16>(bullet.pos.y), 16, 16};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }
  }

  ~Boss() {}
};

#endif
