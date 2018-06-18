#ifndef SNOW_H
#define SNOW_H

#include "def_global.hpp"
#include "image_manager.hpp"

const int snow_count = 256;

class Snow {
  struct Snow_data {
    bool view_p;
    int x;
    int y;
    int type;
  };

  Snow_data snows_[snow_count];
  const ImageManager *image_manager_;

 public:
  Snow(const ImageManager *image_manager) noexcept
      : image_manager_(image_manager) {}

  inline void init() noexcept {
    for (auto &snow : snows_) {
      snow.view_p = false;
    }
  }

  inline void update() noexcept {
    for (auto &snow : snows_) {
      if (snow.view_p) {
        continue;
      }
      snow.view_p = true;
      snow.x = rand() % (screen::width - 8);
      snow.y = -8;
      snow.type = rand() % 2;
      break;
    }

    for (auto &snow : snows_) {
      if (!snow.view_p) {
        continue;
      }
      int move_speed[2] = {12, 8};
      snow.y += move_speed[snow.type];
      if (snow.y >= screen::height) {
        snow.view_p = false;
      }
    }
  }

  inline void draw() const noexcept {
    SDL_Texture *snow_texture = image_manager_->get(image::snow);
    for (const auto &snow : snows_) {
      if (!snow.view_p) {
        continue;
      }
      SDL_Rect dst;
      dst.x = static_cast<Sint16>(snow.x);
      dst.y = static_cast<Sint16>(snow.y);
      SDL_QueryTexture(snow_texture, nullptr, nullptr, &dst.w, &dst.h);
      dst.w /= 2;
      const SDL_Rect src = {dst.w * snow.type, 0, dst.w, dst.h};
      image_manager_->render_copy(*snow_texture, src, dst);
    }
    SDL_DestroyTexture(snow_texture);
  }

  ~Snow() noexcept {}
};

#endif
