//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
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
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;
  Point pos_;
  int life_;
  int move_;
  int shot_rot_;
  int shot_count_;

 public:
  class Bullet {
    bool view_p_;
    Point pos_;
    Point move_;
    int count_;
    int rot_;

   public:
    inline void init() noexcept { view_p_ = false; }

    inline void update() noexcept {
      if (!view_p_) {
        return;
      }
      pos_ += move_;
      if (pos_.x < -16) {
        view_p_ = false;
      }
      if (pos_.y < -16) {
        view_p_ = false;
      }
      if (pos_.x > screen::width) {
        view_p_ = false;
      }
      if (pos_.y > screen::height) {
        view_p_ = false;
      }
      ++count_;
      count_ %= 2;
      if (count_ == 0) {
        ++rot_;
        rot_ %= 16;
      }
    }

    inline void draw(const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }
      SDL_Texture *bullet_texture = image_manager.get(image::bm01);
      SDL_Rect dst;
      dst.x = static_cast<Sint16>(pos_.x);
      dst.y = static_cast<Sint16>(pos_.y);
      SDL_QueryTexture(bullet_texture, nullptr, nullptr, &dst.w, &dst.h);
      const SDL_Rect src = {0, 0, dst.w, dst.h};
      image_manager.render_copy(*bullet_texture, src, dst);
      SDL_DestroyTexture(bullet_texture);
    }

    /**
     * Return false if the bullet has already been shot. Otherwise, return true
     * after making the bullet visible.
     */
    inline bool shoot_attack00(const Point &boss_pos, const int n,
                               const int shot_rot) noexcept {
      if (view_p_) {
        return false;
      }

      const Point p[2] = {{88, 120}, {418 - 88, 120}};
      pos_ = boss_pos + p[n];
      double r = PI * shot_rot / 10;
      if (n == 1) {
        r = -r;
      }
      move_.rot(Point{0, 4}, r);
      view_p_ = true;
      count_ = 0;
      rot_ = 0;

      return true;
    }

    inline void shoot_attack01(const Point &boss_pos, const int i) noexcept {
      pos_ = boss_pos + Point{418 / 2, 105};
      double r = PI * i / 24;
      move_.rot(Point{0, 3}, r);
      view_p_ = true;
      count_ = 0;
      rot_ = 0;
    }

    inline Point get_pos() const noexcept { return pos_; }

    inline void set_pos(const Point &pos) noexcept { pos_ = pos; }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_invisible() noexcept { view_p_ = false; }
  };

  Bullet bullets[BOSS_SHOT_MAX];

  Boss(const ImageManager *image_manager,
       const MixerManager *mixer_manager) noexcept
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {}

  inline void init() noexcept {
    state_ = boss_state::automove;
    pos_ = Point{screen::width / 6, -(screen::height / 2)};
    move_ = 0;
    for (auto &bullet : bullets) {
      bullet.init();
    }
    life_ = 100;
  }

  inline void update() noexcept {
    pos_.x += move_;
    if (pos_.x < 0) {
      pos_.x = 0;
      move_ = 2;
    }
    if ((pos_.x + 400) >= screen::width) {
      pos_.x = screen::width - 400;
      move_ = -2;
    }

    for (auto &bullet : bullets) {
      bullet.update();
    }

    switch (state_) {
      case boss_state::automove: {
        if (pos_.y < 10) {
          pos_.y += 4;
        } else {
          shot_count_ = 0;
          shot_rot_ = 0;
          state_ = boss_state::attack00;
          move_ = -2;
        }
        break;
      }
      case boss_state::attack00: {
        if (!(shot_count_ % 2)) {
          for (int n = 0; n < 2; ++n) {
            for (auto &bullet : bullets) {
              if (!bullet.shoot_attack00(pos_, n, shot_rot_)) {
                continue;
              }
              break;
            }
          }
          ++shot_rot_;
        }
        ++shot_count_;
        if (shot_count_ == 240) {
          state_ = boss_state::attack01;
        }
        break;
      }
      case boss_state::attack01:
      case boss_state::attack02: {
        for (auto &bullet : bullets) {
          if (bullet.view_p()) {
            return;
          }
        }
        for (int i = 0; i < 48; ++i) {
          bullets[i].shoot_attack01(pos_, i);
        }
        Mix_PlayChannel(se_type::enemy_shoot,
                        mixer_manager_->get_se(se_type::enemy_shoot), 0);
        shot_count_ = 0;
        shot_rot_ = 0;
        if (state_ == boss_state::attack01) {
          state_ = boss_state::attack02;
        } else {
          state_ = boss_state::attack00;
        }
        break;
      }
    }
  }

  inline void draw() const noexcept {
    SDL_Texture *boss_texture = image_manager_->get(image::boss);
    SDL_Rect dst;
    dst.x = static_cast<Sint16>(pos_.x);
    dst.y = static_cast<Sint16>(pos_.y);
    SDL_QueryTexture(boss_texture, nullptr, nullptr, &dst.w, &dst.h);
    const SDL_Rect src = {0, 0, dst.w, dst.h};
    image_manager_->render_copy(*boss_texture, src, dst);
    SDL_DestroyTexture(boss_texture);

    for (const auto &bullet : bullets) {
      bullet.draw(*image_manager_);
    }
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Boss() noexcept {}
};

#endif
