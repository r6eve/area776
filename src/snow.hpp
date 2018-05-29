#ifndef SNOW_H
#define SNOW_H

#include "def_global.hpp"
#include "image_manager.hpp"

const int snow_count = 256;

class Snow {
  struct Snow_data {
    bool view;
    int x;
    int y;
    int type;
  };

  Snow_data snows_[snow_count];

 public:
  Snow() noexcept {}

  inline void init() noexcept {
    for (auto &snow : snows_) {
      snow.view = false;
    }
  }

  inline void update() noexcept {
    for (auto &snow : snows_) {
      if (snow.view) {
        continue;
      }
      snow.view = true;
      snow.x = rand() % (screen::width - 8);
      snow.y = -8;
      snow.type = rand() % 2;
      break;
    }

    for (auto &snow : snows_) {
      if (!snow.view) {
        continue;
      }
      int move_speed[2] = {12, 8};
      snow.y += move_speed[snow.type];
      if (snow.y >= screen::height) {
        snow.view = false;
      }
    }
  }

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    for (const auto &snow : snows_) {
      if (!snow.view) {
        continue;
      }
      SDL_Surface *p_surface = image_manager.get(image::snow);
      SDL_Rect src = {static_cast<Sint16>(8 * snow.type), 0, 8, 8};
      SDL_Rect dst = {static_cast<Sint16>(snow.x), static_cast<Sint16>(snow.y),
                      8, 8};
      SDL_BlitSurface(p_surface, &src, screen, &dst);
    }
  }

  ~Snow() noexcept {}
};

#endif
