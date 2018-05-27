#ifndef BG_H
#define BG_H

#include "image_manager.hpp"

struct Snow_data {
  bool view;
  int x;
  int y;
  int type;
};

void init_bg();
void update_bg();
void draw_bg(SDL_Surface *screen, ImageManager &image_manager);

#endif
