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

 public:
  class Bullet {
    Point pos_;
    Point move_;
    bool view_p_;

   public:
    inline void update() noexcept {
      if (!view_p_) {
        return;
      }

      pos_ += move_;
      if (pos_.y < -16) {
        view_p_ = false;
      }
    }

    inline void draw(SDL_Surface *screen,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }

      SDL_Surface *p_surface = image_manager.get(image::oval_re);
      SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                      10, 24};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }

    /**
     * Return false if the bullet has already been shot. Otherwise, return true
     * after making the bullet visible and playing shot sound.
     */
    inline bool shoot(const Point &fighter_pos,
                      const MixerManager &mixer_manager) noexcept {
      if (view_p_) {
        return false;
      }

      view_p_ = true;
      pos_ = fighter_pos + Point{25, 10};
      move_ = Point{0, -16};
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
      return true;
    }

    inline Point get_pos() const noexcept { return pos_; }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_invisible() noexcept { view_p_ = false; }
  };

  Bullet bullets[FIGHTER_SHOT_MAX];

  Fighter() noexcept {}

  inline void init() noexcept {
    pos_ = Point{280, 400};
    shot_timer_ = 0;
    for (auto &bullet : bullets) {
      bullet.make_invisible();
    }
  }

  inline void update(const InputManager &input_manager,
                     const MixerManager &mixer_manager) noexcept {
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

    for (auto &bullet : bullets) {
      bullet.update();
    }

    if (shot_timer_ != 0) {
      --shot_timer_;
      return;
    }

    if (!input_manager.press_key_p(input_device::f)) {
      return;
    }

    for (auto &bullet : bullets) {
      if (bullet.shoot(pos_, mixer_manager)) {
        break;
      }
    }
    shot_timer_ = 8;
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *p_surface = image_manager.get(image::fighter);
    SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                    60, 60};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    for (const auto &bullet : bullets) {
      bullet.draw(screen, image_manager);
    }
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Fighter() noexcept {}
};

#endif
