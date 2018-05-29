#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "effect.hpp"
#include "fighter.hpp"
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

  Enemy() noexcept {}

  inline void init(const bool debug_mode) noexcept {
    for (auto &enemy : enemies) {
      enemy.view = false;
    }
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
    life = debug_mode ? 5 : 30;
  }

  inline void appear(Fighter &fighter) noexcept {
    const double speed = 8;
    if (rand() % 45 != 0) {
      return;
    }

    for (auto &enemy : enemies) {
      if (enemy.view) {
        continue;
      }
      enemy.view = true;
      enemy.pos.x = rand() % (screen::width - 64);
      enemy.pos.y = -64;
      enemy.shot_timer = rand() % 15 + 15;
      enemy.move = fighter.pos - enemy.pos;
      enemy.move.norm();
      enemy.move *= speed;
      break;
    }
  }

  void update(MixerManager &mixer_manager, Fighter &fighter) noexcept;

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
    }
  }

  inline void draw(SDL_Surface *screen, ImageManager &image_manager) const
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
    draw_shot(screen, image_manager);
  }

  ~Enemy() noexcept {}

 private:
  inline void draw_shot(SDL_Surface *screen, ImageManager &image_manager) const
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
