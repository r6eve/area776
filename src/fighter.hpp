#ifndef FIGHTER_H
#define FIGHTER_H

#include <SDL2/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"

class Fighter {
  Point pos_;
  int life_;
  unsigned char shot_timer_;
  const ImageManager *image_manager_;
  const InputManager *input_manager_;
  const MixerManager *mixer_manager_;

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

    inline void draw(const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }

      SDL_Texture *bullet_texture = image_manager.get(image::oval_re);
      SDL_Rect dst;
      dst.x = static_cast<Sint16>(pos_.x);
      dst.y = static_cast<Sint16>(pos_.y);
      SDL_QueryTexture(bullet_texture, nullptr, nullptr, &dst.w, &dst.h);
      SDL_Rect src = {0, 0, dst.w, dst.h};
      image_manager.render_copy(*bullet_texture, src, dst);
      SDL_DestroyTexture(bullet_texture);
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
      Mix_PlayChannel(se_type::fighter_shoot,
                      mixer_manager.get_se(se_type::fighter_shoot), 0);
      return true;
    }

    inline Point get_pos() const noexcept { return pos_; }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_invisible() noexcept { view_p_ = false; }
  };

  Bullet bullets[FIGHTER_SHOT_MAX];

  Fighter(const ImageManager *image_manager, const InputManager *input_manager,
          const MixerManager *mixer_manager) noexcept
      : image_manager_(image_manager),
        input_manager_(input_manager),
        mixer_manager_(mixer_manager) {}

  inline void init() noexcept {
    pos_ = Point{screen::width / 2 - 40, screen::height - 90};
    shot_timer_ = 0;
    for (auto &bullet : bullets) {
      bullet.make_invisible();
    }
  }

  inline void update() noexcept {
    const double move_speed = 4.0;
    if (input_manager_->press_key_p(input_device::up)) {
      pos_.y -= move_speed;
    }
    if (input_manager_->press_key_p(input_device::down)) {
      pos_.y += move_speed;
    }
    if (input_manager_->press_key_p(input_device::left)) {
      pos_.x -= move_speed;
    }
    if (input_manager_->press_key_p(input_device::right)) {
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
    if (pos_.y > (screen::height - 80)) {
      pos_.y = screen::height - 80;
    }

    for (auto &bullet : bullets) {
      bullet.update();
    }

    if (shot_timer_ != 0) {
      --shot_timer_;
      return;
    }

    if (!input_manager_->press_key_p(input_device::f)) {
      return;
    }

    for (auto &bullet : bullets) {
      if (bullet.shoot(pos_, *mixer_manager_)) {
        break;
      }
    }
    shot_timer_ = 8;
  }

  inline void draw() const noexcept {
    SDL_Texture *fighter_texture = image_manager_->get(image::fighter);
    SDL_Rect dst;
    dst.x = static_cast<Sint16>(pos_.x);
    dst.y = static_cast<Sint16>(pos_.y);
    SDL_QueryTexture(fighter_texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_Rect src = {0, 0, dst.w, dst.h};
    image_manager_->render_copy(*fighter_texture, src, dst);
    SDL_DestroyTexture(fighter_texture);
    for (const auto &bullet : bullets) {
      bullet.draw(*image_manager_);
    }
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Fighter() noexcept {}
};

#endif
