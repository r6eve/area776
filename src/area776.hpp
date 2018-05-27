#ifndef AREA776_H
#define AREA776_H

#include "def_global.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "wipe.hpp"

class Area776 {
  const bool debug_mode_;

  Wipe wipe_;
  InputManager input_manager_;
  MixerManager mixer_manager_;

 public:
  Area776(const bool debug_mode) noexcept : debug_mode_(debug_mode) {}
  bool init();
  bool init_sdl();
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
  void draw_translucence();
  ~Area776() {}
};

#endif
