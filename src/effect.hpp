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

    inline void draw(SDL_Renderer *renderer,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }
      const int n = count_ / 2;
      SDL_Texture *effect_texture =
          image_manager.get(renderer, image::effect01);
      SDL_Rect dst = {static_cast<Sint16>(pos_.x), static_cast<Sint16>(pos_.y),
                      effect_size::w, effect_size::h};
      SDL_Rect src = {static_cast<Sint16>(n % 4 * effect_size::w),
                      static_cast<Sint16>(n / 4 * effect_size::h),
                      effect_size::w, effect_size::h};
      SDL_RenderCopy(renderer, effect_texture, &src, &dst);
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

 public:
  Effects() noexcept {}

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

  inline void draw(SDL_Renderer *renderer,
                   const ImageManager &image_manager) const noexcept {
    for (const auto &effect : effects) {
      effect.draw(renderer, image_manager);
    }
  }

  ~Effects() noexcept {}
};

#endif
