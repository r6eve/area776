#include "snow.hpp"
#include "def_global.hpp"
#include "image_manager.hpp"

void Snow::init() {
  for (auto &snow : snows_) {
    snow.view = false;
  }
}

void Snow::update() {
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

void Snow::draw(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &snow : snows_) {
    if (!snow.view) {
      continue;
    }
    SDL_Surface *p_surface = image_manager.get(image::snow);
    SDL_Rect src = {static_cast<Sint16>(8 * snow.type), 0, 8,
                    8};  // TODO: 8 is a size of snow.
    SDL_Rect dst = {static_cast<Sint16>(snow.x), static_cast<Sint16>(snow.y), 8,
                    8};
    SDL_BlitSurface(p_surface, &src, screen, &dst);
  }
}
