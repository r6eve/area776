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

 public:
  Snow() noexcept {}

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

  inline void draw(SDL_Surface *screen, const ImageManager &image_manager) const
      noexcept {
    for (const auto &snow : snows_) {
      if (!snow.view_p) {
        continue;
      }
      SDL_Surface *p_surface = image_manager.get(image::snow);
      const Uint16 w = static_cast<Uint16>(p_surface->w / 2);
      SDL_Rect src = {static_cast<Sint16>(w * snow.type), 0, w,
                      static_cast<Uint16>(p_surface->h)};
      SDL_Rect dst = {static_cast<Sint16>(snow.x), static_cast<Sint16>(snow.y),
                      w, static_cast<Uint16>(p_surface->h)};
      SDL_BlitSurface(p_surface, &src, screen, &dst);
    }
  }

  ~Snow() noexcept {}
};

#endif
