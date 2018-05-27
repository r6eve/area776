#ifndef AREA776_H
#define AREA776_H

#include "def_global.hpp"

class Area776 {
public:
  Area776() {}
  bool init();
  bool init_sdl();
  bool init_audio();
  bool init_font();
  bool init_game();
  void main_loop();
  void title();
  void game_start();
  void play_game();
  void game_clear();
  void game_over();
  void game_pause();
  void draw_life();
  bool poll_event();
  void wait_game();
  void draw_fps();
  void end();
  void draw_map();
  void end_audio();
  void draw_translucence();
  ~Area776() {}
};

#endif
