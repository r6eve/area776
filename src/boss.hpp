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
  Point pos_;
  int life_;
  int move_;
  int shot_rot_;
  int shot_count_;

 public:
  class Bullet {
    Point pos_;
    bool view_p_;

   public:
    int rot;
    int count;
    Point move;

    inline void init() noexcept { view_p_ = false; }

    inline void update() noexcept {
      if (!view_p_) {
        return;
      }
      pos_ += move;
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
      ++count;
      count %= 2;
      if (count == 0) {
        ++rot;
        rot %= 16;
      }
    }

    inline void draw(SDL_Surface *screen,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }
      SDL_Surface *p_surface = image_manager.get(image::bm01);
      SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                      static_cast<Uint16>(p_surface->w),
                      static_cast<Uint16>(p_surface->h)};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }

    inline Point get_pos() const noexcept { return pos_; }

    inline void set_pos(const Point &pos) noexcept { pos_ = pos; }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_visible() noexcept { view_p_ = true; }

    inline void make_invisible() noexcept { view_p_ = false; }
  };

  Bullet bullets[BOSS_SHOT_MAX];

  Boss() noexcept {}

  inline void init() noexcept {
    state_ = boss_state::automove;
    pos_ = Point{screen::width / 6, -(screen::height / 2)};
    move_ = 0;
    for (auto &bullet : bullets) {
      bullet.init();
    }
    life_ = 100;
  }

  inline void update(const MixerManager &mixer_manager) noexcept {
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
          const Point p[2] = {{88, 120}, {418 - 88, 120}};
          for (int n = 0; n < 2; ++n) {
            for (auto &bullet : bullets) {
              if (bullet.view_p()) {
                continue;
              }
              bullet.set_pos(pos_ + p[n]);
              double r = PI * shot_rot_ / 10;
              if (n == 1) {
                r = -r;
              }
              bullet.move.rot(Point{0, 4}, r);
              bullet.make_visible();
              bullet.count = 0;
              bullet.rot = 0;
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
          bullets[i].set_pos(pos_ + Point{418 / 2, 105});
          double r = PI * i / 24;
          bullets[i].move.rot(Point{0, 3}, r);
          bullets[i].make_visible();
          bullets[i].count = 0;
          bullets[i].rot = 0;
        }
        Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
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

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *p_surface = image_manager.get(image::boss);
    SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                    static_cast<Uint16>(p_surface->w),
                    static_cast<Uint16>(p_surface->h)};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);

    for (const auto &bullet : bullets) {
      bullet.draw(screen, image_manager);
    }
  }

  inline Point get_pos() const noexcept { return pos_; }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Boss() noexcept {}
};

#endif
