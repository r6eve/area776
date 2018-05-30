#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
#include "effect.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

class Boss {
  enum class boss_state {
    automove,
    attack00,
    attack01,
    attack02,
  };

  boss_state state_;
  Point pos_;
  int life_;
  int move_;
  int shot_rot_;
  int shot_count_;

  inline void draw_shot(SDL_Surface *screen,
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

 public:
  struct Bullet {
    bool view;
    int rot;
    int count;
    Point pos;
    Point move;
  };

  Bullet bullets[BOSS_SHOT_MAX];

  Boss() noexcept {}

  inline void init() noexcept {
    state_ = boss_state::automove;
    pos_ = Point{(screen::width - 418) / 2, -240};
    move_ = 0;
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
    life_ = 100;
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
    SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                    400, 224};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    draw_shot(screen, image_manager);
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Boss() noexcept {}
};

#endif
