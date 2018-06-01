#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

class Enemies {
  int life_;

  inline void draw_bullets(SDL_Surface *screen,
                           const ImageManager &image_manager) const noexcept {
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

  inline void appear(const Fighter &fighter) noexcept {
    if (rand() % 45 != 0) {
      return;
    }

    const double speed = 8;
    for (auto &enemy : enemies) {
      if (enemy.view) {
        continue;
      }
      enemy.view = true;
      enemy.pos.x = rand() % (screen::width - 64);
      enemy.pos.y = -64;
      enemy.shot_timer = rand() % 15 + 15;
      enemy.move = fighter.get_pos() - enemy.pos;
      enemy.move.norm();
      enemy.move *= speed;
      break;
    }
  }

  inline void fade_bullets() noexcept {
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
    }
  }

 public:
  struct Enemy {
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

  Enemy enemies[ENEMY_MAX];
  Bullet bullets[ENEMY_SHOT_MAX];

  Enemies() noexcept {}

  inline void init(const bool debug_mode) noexcept {
    for (auto &enemy : enemies) {
      enemy.view = false;
    }
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
    life_ = debug_mode ? 5 : 30;
  }

  void update(const MixerManager &mixer_manager,
              const Fighter &fighter) noexcept;

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    for (const auto &enemy : enemies) {
      if (!enemy.view) {
        continue;
      }

      SDL_Surface *p_surface = image_manager.get(image::mons13);
      SDL_Rect dst = {static_cast<Sint16>(enemy.pos.x),
                      static_cast<Sint16>(enemy.pos.y), 35, 35};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }
    draw_bullets(screen, image_manager);
  }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Enemies() noexcept {}
};

#endif
