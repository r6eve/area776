#ifndef EFFECT_H
#define EFFECT_H

#include "def_global.hpp"
#include "image_manager.hpp"

namespace effect_size {
enum {
  w = 160,
  h = 160,
};
}  // namespace effect_size

struct Effects {
  class Effect {
    bool view_p_;
    Point pos_;
    int count_;

   public:
    inline void init() noexcept { view_p_ = false; }

    inline void update() noexcept {
      if (!view_p_) {
        return;
      }
      ++count_;
      if (count_ >= 24) {
        view_p_ = false;
      }
    }

    inline void draw(SDL_Surface *screen,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }
      const int n = count_ / 2;
      SDL_Surface *p_surface = image_manager.get(image::effect01);
      SDL_Rect src = {static_cast<Sint16>(n % 4 * effect_size::w),
                      static_cast<Sint16>(n / 4 * effect_size::h),
                      effect_size::w, effect_size::h};
      SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                      effect_size::w, effect_size::h};
      SDL_BlitSurface(p_surface, &src, screen, &dst);
    }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_visible(const Point &pos) noexcept {
      view_p_ = true;
      pos_ = pos;
      count_ = 0;
    }

    inline Point get_pos() const noexcept { return pos_; }
  };

  Effect effects[EFFECT_MAX];

  inline void init() noexcept {
    for (auto &effect : effects) {
      effect.init();
    }
  }

  inline void update() noexcept {
    for (auto &effect : effects) {
      effect.update();
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    for (const auto &effect : effects) {
      effect.draw(screen, image_manager);
    }
  }
};

#endif
