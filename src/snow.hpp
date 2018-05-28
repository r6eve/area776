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
  Snow() {}
  void init();
  void update();
  void draw(SDL_Surface *screen, ImageManager &image_manager);
  ~Snow() {}
};

#endif
