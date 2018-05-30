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

   public:
    bool view;
    int rot;
    int count;
    Point move;

    inline void init() noexcept { view = false; }

    inline void update() noexcept {
      if (!view) {
        return;
      }
      pos_ += move;
      if (pos_.x < -16) {
        view = false;
      }
      if (pos_.y < -16) {
        view = false;
      }
      if (pos_.x > screen::width) {
        view = false;
      }
      if (pos_.y > screen::height) {
        view = false;
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
      if (!view) {
        return;
      }
      SDL_Surface *p_surface = image_manager.get(image::bm01);
      SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                      16, 16};
      SDL_BlitSurface(p_surface, nullptr, screen, &dst);
    }

    inline Point get_pos() const noexcept { return pos_; }

    inline void set_pos(const Point &pos) noexcept { pos_ = pos; }
  };

  Bullet bullets[BOSS_SHOT_MAX];

  Boss() noexcept {}

  inline void init() noexcept {
    state_ = boss_state::automove;
    pos_ = Point{(screen::width - 418) / 2, -240};
    move_ = 0;
    for (auto &bullet : bullets) {
      bullet.init();
    }
    life_ = 100;
  }

  void update(const MixerManager &mixer_manager) noexcept;

  inline void update_bullets() noexcept {
    for (auto &bullet : bullets) {
      bullet.update();
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    SDL_Surface *p_surface = image_manager.get(image::boss);
    SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                    400, 224};
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
