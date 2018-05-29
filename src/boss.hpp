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
  Point pos;
  int move;
  int shot_rot;
  int shot_count;
  int life;
  Bullet bullets[BOSS_SHOT_MAX];

  Boss() noexcept {}

  inline void init() noexcept {
    state = boss_state::automove;
    pos = Point{(screen::width - 418) / 2, -240};
    move = 0;
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
    life = 100;
  }

  void update(const MixerManager &mixer_manager) noexcept;

  inline void update_shot() noexcept {
    for (auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }
      bullet.pos += bullet.move;
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

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *p_surface = image_manager.get(image::boss);
    SDL_Rect dst = {static_cast<Sint16>(pos.x), static_cast<Sint16>(pos.y), 400,
                    224};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    draw_shot(screen, image_manager);
  }

  ~Boss() noexcept {}

 private:
  inline void draw_shot(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    for (const auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }
      SDL_Surface *p_surface = image_manager.get(image::bm01);
      SDL_Rect dst = {static_cast<Sint16>(bullet.pos.x),
                      static_cast<Sint16>(bullet.pos.y), 16, 16};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }
  }
};

#endif
