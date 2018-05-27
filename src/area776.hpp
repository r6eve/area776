#ifndef AREA776_H
#define AREA776_H

#include "def_global.hpp"

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
bool check_hit_rect(SDL_Rect *a, SDL_Rect *b);
void end_audio();
void draw_translucence();

#endif
