#ifndef FIGHTER_H
#define FIGHTER_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"

class Fighter {
  Point pos_;
  int life_;
  int shot_timer_;

  inline void draw_shot(SDL_Surface *screen,
                        const ImageManager &image_manager) const noexcept {
    SDL_Surface *p_surface = image_manager.get(image::oval_re);
    for (const auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }

      SDL_Rect dst = {static_cast<Sint16>(bullet.pos.x),
                      static_cast<Sint16>(bullet.pos.y), 10, 24};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }
  }

 public:
  struct Bullet {
    bool view;
    Point pos;
    Point move;
  };

  Bullet bullets[FIGHTER_SHOT_MAX];

  Fighter() noexcept {}

  inline void init() noexcept {
    pos_ = Point{280, 400};
    shot_timer_ = 0;
    for (auto &bullet : bullets) {
      bullet.view = false;
    }
  }

  void update(const InputManager &input_manager,
              const MixerManager &mixer_manager) noexcept;

  inline void update_shot() noexcept {
    for (auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }

      bullet.pos += bullet.move;
      if (bullet.pos.y < -16) {
        bullet.view = false;
      }
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *p_surface = image_manager.get(image::fighter);
    SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                    60, 60};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    draw_shot(screen, image_manager);
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Fighter() noexcept {}
};

#endif
