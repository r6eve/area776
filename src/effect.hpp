//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

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

class Effects {
  class Effect {
    bool view_p_;
    Point pos_;
    int count_;

   public:
    Effect() noexcept {}

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

    inline void draw(const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }
      const int n = count_ / 2;
      SDL_Texture *effect_texture = image_manager.get(image::effect01);
      const SDL_Rect dst = {static_cast<Sint16>(pos_.x),
                            static_cast<Sint16>(pos_.y), effect_size::w,
                            effect_size::h};
      const SDL_Rect src = {static_cast<Sint16>(n % 4 * effect_size::w),
                            static_cast<Sint16>(n / 4 * effect_size::h),
                            effect_size::w, effect_size::h};
      image_manager.render_copy(*effect_texture, src, dst);
      SDL_DestroyTexture(effect_texture);
    }

    /**
     * Return false if the effect has already been visible, and true otherwise.
     */
    inline bool make_visible(const Point &pos) noexcept {
      if (view_p_) {
        return false;
      }
      view_p_ = true;
      pos_ = pos;
      count_ = 0;
      return true;
    }

    ~Effect() noexcept {}
  };

  Effect effects[EFFECT_MAX];
  const ImageManager *image_manager_;

 public:
  Effects(const ImageManager *image_manager) noexcept
      : image_manager_(image_manager) {}

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

  inline void make_visible(const Point &pos) noexcept {
    for (auto &effect : effects) {
      if (effect.make_visible(pos)) {
        return;
      }
    }
  }

  inline void draw() const noexcept {
    for (const auto &effect : effects) {
      effect.draw(*image_manager_);
    }
  }

  ~Effects() noexcept {}
};

#endif
