#define MAIN

#include "area776.hpp"
#include <SDL/SDL_mixer.h>
#include <iomanip>
#include <sstream>
#include "def_global.hpp"
#include "image_manager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bg.hpp"
#include "boss.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "vector.hpp"
#include "wipe.hpp"

static int Blink_count;

bool Area776::init() {
  if (!init_sdl()) {
    return false;
  }
  Game_count = 0;
  Game_state = GAME_STATE_TITLE;
  Blink_count = 0;

  return true;
}

bool Area776::init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "Can't initialize sdl. : %s\n", SDL_GetError());
    return false;
  }
  SDL_WM_SetCaption("SDL_SHOOTING", NULL);
  if (debug_mode_) {
    screen_ = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                               SDL_HWSURFACE | SDL_DOUBLEBUF);
  } else {
    screen_ = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                               SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  }
  if (!screen_) {
    fprintf(stderr, "Can't initialize screen. : %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }
  SDL_ShowCursor(SDL_DISABLE);

  return true;
}

void Area776::main_loop() {
  for (;;) {
    input_manager_.update();
    switch (Game_state) {
      case GAME_STATE_TITLE:
        game_title();
        break;
      case GAME_STATE_START:
        game_start();
        break;
      case GAME_STATE_GAME:
        play_game();
        break;
      case GAME_STATE_CLEAR:
        game_clear();
        break;
      case GAME_STATE_OVER:
        game_over();
        break;
      case GAME_STATE_PAUSE:
        game_pause();
        break;
    }
    if (!poll_event()) {
      return;
    }
    if (debug_mode_) {
      draw_fps();
    }
    SDL_Flip(screen_);
    wait_game();
  }
}

void Area776::game_title() {
  SDL_Rect dst_back = {0, 0, screen::width, screen::height};
  Uint32 bg_col = 0x45402b;
  SDL_FillRect(screen_, &dst_back, bg_col);
  const Point title_pos = Point{160, 180};
  const Point message_pos = Point{210, 300};
  const char *title_str = "A  r  e  a  7  7  6";
  const char *message_str = "P r e s s  S p a c e  K e y";
  switch (Game_count) {
    case 0:
      wipe_.set_wipe_in();
      ++Game_count;
      break;
    case 1:
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      wipe_.draw(screen_);
      if (wipe_.update()) {
        ++Game_count;
      }
      break;
    case 2:
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      ++Blink_count;
      if (Blink_count >= 30) {
        SDL_Rect dst_back = {static_cast<Sint16>(message_pos.x),
                             static_cast<Sint16>(message_pos.y),
                             screen::width - 240, screen::height - 300};
        SDL_FillRect(screen_, &dst_back, bg_col);
        if (Blink_count >= 60) {
          Blink_count = 0;
        }
      }
      if (input_manager_.press_key_p(input_device::space)) {
        wipe_.set_wipe_out();
        ++Game_count;
      }
      break;
    case 3:
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      wipe_.draw(screen_);
      if (wipe_.update()) {
        Mix_PlayMusic(mixer_manager_.get_music(), -1);
        init_fighter();
        init_enemy();
        init_effect();
        init_bg();
        init_boss();
        Game_count = 0;
        Game_state = GAME_STATE_START;
        Game_level = 1;
        Enemy_select = ENEMY_1;
        Chara_life = 20;
        Enemy_life = 0;
        Boss_life = 0;
        srand((unsigned int)time(NULL));
      }
      break;
  }
}

void Area776::game_start() {
  move_fighter(input_manager_, mixer_manager_);
  move_fighter_shot();
  draw_map();
  draw_fighter(screen_, image_manager_);
  draw_fighter_shot(screen_, image_manager_);

  if (Game_count == 0) {
    wipe_.set_wipe_in();
    wipe_.draw(screen_);
    ++Game_count;
  } else if (Game_count == 1) {
    wipe_.draw(screen_);
    if (wipe_.update()) {
      ++Game_count;
    }
  } else {
    ++Game_count;
  }

  if (Game_count < 130) {
    std::stringstream ss;
    ss << "S t a g e " << Game_level;
    draw_text(font_size::x36, rgb::red, Point{210, 180}, ss.str().c_str());
  } else if (Game_count < 200) {
    draw_text(font_size::x36, rgb::red, Point{220, 180}, "S t a r t");
  }

  if (Game_count > 220) {
    Game_count = 0;
    Game_state = GAME_STATE_GAME;
  }
}

void Area776::play_game() {
  move_fighter(input_manager_, mixer_manager_);
  move_fighter_shot();
  check_enemyshots_hit_mychara(); /* do GameSelect = GAME_STATE_OVER;  */
  update_effect();
  draw_map();
  if (Enemy_select == ENEMY_1) {
    appear_enemy();
    move_enemy(mixer_manager_);
    move_enemy_shot();
    check_myshots_hit_enemy(); /* do Enemy_select = BOSS_1; */
    update_bg();
    draw_bg(screen_, image_manager_);
    draw_enemy(screen_, image_manager_);
    draw_enemy_shot(screen_, image_manager_);
  } else if (Enemy_select == BOSS_1) {
    if ((Game_count < 130) && (Blink_count < 20)) {
      std::stringstream ss;
      ss << "B O O S  " << Game_level;
      draw_text(font_size::x36, rgb::red, Point{210, 180}, ss.str().c_str());
      ++Game_count;
      ++Blink_count;
      if (Blink_count >= 20) {
        if (Blink_count >= 40) {
          Blink_count = 0;
        }
      }
    } else if ((Game_count < 130) && (Blink_count >= 20)) {
      if (Blink_count >= 40) {
        Blink_count = 0;
      }
      ++Game_count;
      ++Blink_count;
    } else {
      move_boss(mixer_manager_);
      move_boss_shot();

      /* In this function, Game_state = GAME_STATE_CLEAR; Game_count = 0 */
      check_myshots_hit_boss();

      draw_boss(screen_, image_manager_);
      draw_boss_shot(screen_, image_manager_);
    }
  }
  draw_fighter_shot(screen_, image_manager_);
  draw_fighter(screen_, image_manager_);
  draw_effect(screen_, image_manager_);
  draw_life();
}

void Area776::game_clear() {
  move_fighter(input_manager_, mixer_manager_);
  move_fighter_shot();
  draw_map();
  draw_fighter(screen_, image_manager_);
  draw_fighter_shot(screen_, image_manager_);
  draw_life();

  if (Game_count == 0) {
    wipe_.set_wipe_out();
    wipe_.draw(screen_);
    ++Game_count;
  } else if (Game_count >= 1) {
    wipe_.draw(screen_);
    if (wipe_.update()) {
      if (Game_level == 1) {
        SDL_Rect dst_back = {0, 0, screen::width, screen::height};
        Uint32 col = 0xffffffff;
        SDL_FillRect(screen_, &dst_back, col);
        draw_text(font_size::x36, rgb::red, Point{200, 180},
                  "G A M E  C L E A R");
        ++Game_count;
        if (Game_count > 200) {
          wipe_.draw(screen_);
          if (wipe_.update()) {
            Game_count = 0;
            Game_state = GAME_STATE_TITLE;
            Mix_HaltMusic();
          }
        }
      } else {
        init_fighter();
        init_enemy();
        init_boss();
        init_effect();
        init_bg();
        Game_count = 0;
        Game_state = GAME_STATE_START;
        ++Game_level;
        Enemy_life = 0;
        Boss_life = 0;
      }
    }
  }
}

void Area776::game_over() {
  draw_text(font_size::x36, rgb::red, Point{200, 180}, "G a m e O v e r");
  ++Game_count;
  if (Game_count > 200) {
    wipe_.draw(screen_);
    if (wipe_.update()) {
      Game_count = 0;
      Game_state = GAME_STATE_TITLE;
      Mix_HaltMusic();
    }
  }
}

void Area776::game_pause() {
  draw_map();
  if (Enemy_select == ENEMY_1) {
    draw_bg(screen_, image_manager_);
    draw_enemy(screen_, image_manager_);
    draw_enemy_shot(screen_, image_manager_);
  } else if (Enemy_select == BOSS_1) {
    draw_boss(screen_, image_manager_);
    draw_boss_shot(screen_, image_manager_);
  }
  draw_fighter_shot(screen_, image_manager_);
  draw_fighter(screen_, image_manager_);
  draw_effect(screen_, image_manager_);
  draw_life();
  draw_translucence();
  if (input_manager_.edge_key_p(input_device::space)) {
    Game_state = GAME_STATE_GAME;
  }
}

void Area776::draw_life() {
  if (Enemy_select == ENEMY_1) {
    std::stringstream ss;
    ss << "ENEMY LIFE:  " << 30 - Enemy_life;
    draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
  } else if (Enemy_select == BOSS_1) {
    std::stringstream ss;
    ss << "BOSS LIFE:  " << 100 - Boss_life;
    draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
  }
  std::stringstream ss;
  ss << "LIFE:  " << Chara_life;
  draw_text(font_size::x16, rgb::white,
            Point{static_cast<int>(Fighter.pos.x),
                  static_cast<int>(Fighter.pos.y + 55)},
            ss.str().c_str());
}

bool Area776::poll_event() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;
    }
  }
  return true;
}

void Area776::wait_game() {
  static Uint32 pre_count;
  double wait_time = 1000.0 / screen::max_fps;
  Uint32 wait_count = (wait_time + 0.5);
  if (pre_count) {
    Uint32 now_count = SDL_GetTicks();
    Uint32 interval = now_count - pre_count;
    if (interval < wait_count) {
      Uint32 delay_time = wait_count - interval;
      SDL_Delay(delay_time);
    }
  }
  pre_count = SDL_GetTicks();
}

void Area776::draw_fps() {
  static Uint32 pre_count;
  Uint32 now_count = SDL_GetTicks();
  if (pre_count) {
    Uint32 interval = now_count - pre_count;
    if (interval < 1) {
      interval = 1;
    }
    double frame_rate = 1000.0 / interval;
    std::stringstream ss;
    ss << "FrameRate[" << std::setprecision(2)
       << std::setiosflags(std::ios::fixed) << frame_rate << "]";
    draw_text(font_size::x16, rgb::green, Point{screen::width - 140, 16},
              ss.str().c_str());
  }
  pre_count = now_count;
}

void Area776::draw_map() {
  SDL_Surface *pSurface = image_manager_.get(image::map);
  SDL_Rect dst = {0, 0};
  SDL_BlitSurface(pSurface, NULL, screen_, &dst);
}

void Area776::draw_translucence() {
  Uint32 rmask, gmask, bmask;
  Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
#endif
  SDL_Rect dst_back;
  dst_back.x = 0;
  dst_back.y = 0;

  SDL_Surface *trans_surface = SDL_CreateRGBSurface(
      SDL_SWSURFACE, screen::width, screen::height, 32, rmask, gmask, bmask, 0);
  if (trans_surface == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, NULL, screen_, &dst_back);
  if (Blink_count < 30) {
    draw_text(font_size::x36, rgb::white, Point{220, 180}, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}
