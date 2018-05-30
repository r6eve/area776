#ifndef FIGHTER_H
#define FIGHTER_H

#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
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

  inline void update(const InputManager &input_manager) noexcept {
    const double move_speed = 4.0;
    if (input_manager.press_key_p(input_device::up)) {
      pos_.y -= move_speed;
    }
    if (input_manager.press_key_p(input_device::down)) {
      pos_.y += move_speed;
    }
    if (input_manager.press_key_p(input_device::left)) {
      pos_.x -= move_speed;
    }
    if (input_manager.press_key_p(input_device::right)) {
      pos_.x += move_speed;
    }

    if (pos_.x < 0) {
      pos_.x = 0;
    }
    if (pos_.y < 0) {
      pos_.y = 0;
    }
    if (pos_.x > (screen::width - 60)) {
      pos_.x = screen::width - 60;
    }
    if (pos_.y > (screen::height - 70)) {
      pos_.y = screen::height - 70;
    }
  }

  inline void update_shot(const InputManager &input_manager,
                          const MixerManager &mixer_manager) noexcept {
    for (auto &bullet : bullets) {
      if (!bullet.view) {
        continue;
      }

      bullet.pos += bullet.move;
      if (bullet.pos.y < -16) {
        bullet.view = false;
      }
    }

    if (shot_timer_ != 0) {
      --shot_timer_;
      return;
    }

    if (!input_manager.press_key_p(input_device::f)) {
      return;
    }

    const double shot_speed = 16;
    for (auto &bullet : bullets) {
      if (bullet.view) {
        continue;
      }

      bullet.view = true;
      bullet.pos = pos_ + Point{25, 10};
      bullet.move = Point{0, -shot_speed};
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
      break;
    }
    shot_timer_ = 8;
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
