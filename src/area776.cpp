#define MAIN

#include "area776.hpp"
#include <SDL/SDL_mixer.h>
#include <time.h>
#include <iomanip>
#include <sstream>
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "snow.hpp"
#include "util.hpp"
#include "wipe.hpp"

void Area776::run() {
  for (;;) {
    input_manager_.update();
    switch (game_state_) {
      case game_state::title:
        game_title();
        break;
      case game_state::start:
        game_start();
        break;
      case game_state::playing:
        play_game();
        break;
      case game_state::clear:
        game_clear();
        break;
      case game_state::gameover:
        game_over();
        break;
      case game_state::pause:
        game_pause();
        break;
      default:
        // NOTREACHED
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
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  const Uint32 bg_col = 0x45402b;
  SDL_FillRect(screen_, &dst, bg_col);
  const Point title_pos = Point{160, 180};
  const Point message_pos = Point{210, 300};
  const char *title_str = "A  r  e  a  7  7  6";
  const char *message_str = "P r e s s  S p a c e  K e y";
  switch (game_count_) {
    case 0: {
      wipe_.set_wipe_in();
      ++game_count_;
      break;
    }
    case 1: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      wipe_.draw(screen_);
      if (wipe_.update()) {
        ++game_count_;
      }
      break;
    }
    case 2: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      ++blink_count_;
      if (blink_count_ >= 30) {
        SDL_Rect dst_back = {static_cast<Sint16>(message_pos.x),
                             static_cast<Sint16>(message_pos.y),
                             screen::width - 240, screen::height - 300};
        SDL_FillRect(screen_, &dst_back, bg_col);
        if (blink_count_ >= 60) {
          blink_count_ = 0;
        }
      }
      if (input_manager_.press_key_p(input_device::space)) {
        wipe_.set_wipe_out();
        ++game_count_;
      }
      break;
    }
    case 3: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      wipe_.draw(screen_);
      if (wipe_.update()) {
        Mix_PlayMusic(mixer_manager_.get_music(), -1);
        fighter_.init();
        enemy_.init();
        effect_.init();
        snow_.init();
        boss_.init();
        game_count_ = 0;
        game_state_ = game_state::start;
        game_level_ = 1;
        enemy_select_ = enemy_type::enemy;
        fighter_.life = 20;
        enemy_.life = 0;
        boss_.life = 0;
        srand((unsigned int)time(nullptr));
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void Area776::game_start() {
  fighter_.update(input_manager_, mixer_manager_);
  fighter_.update_shot();
  draw_map();
  fighter_.draw(screen_, image_manager_);
  fighter_.draw_shot(screen_, image_manager_);

  switch (game_count_) {
    case 0: {
      wipe_.set_wipe_in();
      wipe_.draw(screen_);
      ++game_count_;
      break;
    }
    case 1: {
      wipe_.draw(screen_);
      if (wipe_.update()) {
        ++game_count_;
      }
      break;
    }
    default:
      ++game_count_;
      break;
  }

  if (game_count_ < 130) {
    std::stringstream ss;
    ss << "S t a g e " << game_level_;
    draw_text(font_size::x36, rgb::red, Point{210, 180}, ss.str().c_str());
  } else if (game_count_ < 200) {
    draw_text(font_size::x36, rgb::red, Point{220, 180}, "S t a r t");
  }

  if (game_count_ > 220) {
    game_count_ = 0;
    game_state_ = game_state::playing;
  }
}

void Area776::play_game() {
  if (input_manager_.edge_key_p(input_device::space)) {
    game_state_ = game_state::pause;
  }

  fighter_.update(input_manager_, mixer_manager_);
  fighter_.update_shot();
  if (util::check_enemyshots_hit_fighter(enemy_select_, fighter_, enemy_, boss_,
                                         effect_)) {
    game_state_ = game_state::gameover;
  }
  effect_.update();
  draw_map();

  switch (enemy_select_) {
    case enemy_type::enemy: {
      enemy_.appear(fighter_);
      enemy_.update(mixer_manager_, fighter_);
      enemy_.update_shot();
      if (util::check_fightershots_hit_enemy(fighter_, enemy_, effect_)) {
        enemy_select_ = enemy_type::boss;
      }
      snow_.update();
      snow_.draw(screen_, image_manager_);
      enemy_.draw(screen_, image_manager_);
      enemy_.draw_shot(screen_, image_manager_);
      break;
    }
    case enemy_type::boss: {
      if (game_count_ < 130) {
        if (blink_count_ < 20) {
          std::stringstream ss;
          ss << "B O O S  " << game_level_;
          draw_text(font_size::x36, rgb::red, Point{210, 180},
                    ss.str().c_str());
          ++game_count_;
          ++blink_count_;
        } else {
          if (blink_count_ >= 40) {
            blink_count_ = 0;
          }
          ++game_count_;
          ++blink_count_;
        }
      } else {
        boss_.update(mixer_manager_);
        boss_.update_shot();

        if (util::check_fightershots_hit_boss(fighter_, boss_, effect_)) {
          game_state_ = game_state::clear;
          game_count_ = 0;
        }

        boss_.draw(screen_, image_manager_);
        boss_.draw_shot(screen_, image_manager_);
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }

  fighter_.draw_shot(screen_, image_manager_);
  fighter_.draw(screen_, image_manager_);
  effect_.draw(screen_, image_manager_);
  draw_life();
}

void Area776::game_clear() {
  fighter_.update(input_manager_, mixer_manager_);
  fighter_.update_shot();
  draw_map();
  fighter_.draw(screen_, image_manager_);
  fighter_.draw_shot(screen_, image_manager_);
  draw_life();

  if (game_count_ == 0) {
    wipe_.set_wipe_out();
    wipe_.draw(screen_);
    ++game_count_;
    return;
  }

  wipe_.draw(screen_);
  if (!wipe_.update()) {
    return;
  }

  if (game_level_ == 1) {
    SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_FillRect(screen_, &dst, 0xffffffff);
    draw_text(font_size::x36, rgb::red, Point{150, 180}, "G A M E  C L E A R");
    ++game_count_;
    if (game_count_ > 200) {
      wipe_.draw(screen_);
      if (wipe_.update()) {
        game_count_ = 0;
        game_state_ = game_state::title;
        Mix_HaltMusic();
      }
    }
    return;
  }

  fighter_.init();
  enemy_.init();
  boss_.init();
  effect_.init();
  snow_.init();
  game_count_ = 0;
  game_state_ = game_state::start;
  ++game_level_;
  enemy_.life = 0;
  boss_.life = 0;
}

void Area776::game_over() {
  draw_text(font_size::x36, rgb::red, Point{200, 180}, "G a m e O v e r");
  ++game_count_;
  if (game_count_ <= 200) {
    return;
  }

  wipe_.draw(screen_);
  if (!wipe_.update()) {
    return;
  }

  game_count_ = 0;
  game_state_ = game_state::title;
  Mix_HaltMusic();
}

void Area776::game_pause() {
  draw_map();

  switch (enemy_select_) {
    case enemy_type::enemy: {
      snow_.draw(screen_, image_manager_);
      enemy_.draw(screen_, image_manager_);
      enemy_.draw_shot(screen_, image_manager_);
      break;
    }
    case enemy_type::boss: {
      boss_.draw(screen_, image_manager_);
      boss_.draw_shot(screen_, image_manager_);
      break;
    }
    default:
      // NOTREACHED
      break;
  }

  fighter_.draw_shot(screen_, image_manager_);
  fighter_.draw(screen_, image_manager_);
  effect_.draw(screen_, image_manager_);
  draw_life();
  draw_translucence();
  if (input_manager_.edge_key_p(input_device::space)) {
    game_state_ = game_state::playing;
  }
}

void Area776::draw_life() {
  switch (enemy_select_) {
    case enemy_type::enemy: {
      std::stringstream ss;
      ss << "ENEMY LIFE:  " << 30 - enemy_.life;
      draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
      break;
    }
    case enemy_type::boss: {
      std::stringstream ss;
      ss << "BOSS LIFE:  " << 100 - boss_.life;
      draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
      break;
    }
    default:
      // NOTREACHED
      break;
  }

  std::stringstream ss;
  ss << "LIFE:  " << fighter_.life;
  draw_text(font_size::x16, rgb::white,
            Point{fighter_.pos.x, fighter_.pos.y + 55}, ss.str().c_str());
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
      default:
        // do nothing
        break;
    }
  }
  return true;
}

void Area776::wait_game() {
  static Uint32 pre_count;
  const double wait_time = 1000.0 / screen::max_fps;
  const Uint32 wait_count = (wait_time + 0.5);
  if (pre_count) {
    const Uint32 now_count = SDL_GetTicks();
    const Uint32 interval = now_count - pre_count;
    if (interval < wait_count) {
      const Uint32 delay_time = wait_count - interval;
      SDL_Delay(delay_time);
    }
  }
  pre_count = SDL_GetTicks();
}

void Area776::draw_fps() {
  static Uint32 pre_count;
  const Uint32 now_count = SDL_GetTicks();
  if (pre_count) {
    static double frame_rate;
    Uint32 mut_interval = now_count - pre_count;
    if (mut_interval < 1) {
      mut_interval = 1;
    }
    const Uint32 interval = mut_interval;

    if (!(pre_count % 30)) {
      frame_rate = 1000.0 / interval;
    }

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
  SDL_Rect dst = {0, 0, screen::width, screen::height};
  SDL_BlitSurface(pSurface, nullptr, screen_, &dst);
}

void Area776::draw_translucence() {
  Uint32 rmask, gmask, bmask, amask;
  Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface *trans_surface =
      SDL_CreateRGBSurface(SDL_SWSURFACE, screen::width, screen::height, 32,
                           rmask, gmask, bmask, amask);
  if (trans_surface == nullptr) {
    std::cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_Rect dst_back;
  dst_back.x = 0;
  dst_back.y = 0;
  SDL_BlitSurface(trans_surface, nullptr, screen_, &dst_back);
  if (blink_count_ < 30) {
    draw_text(font_size::x36, rgb::white, Point{220, 180}, "P a u s e");
    ++blink_count_;
  } else if (blink_count_ < 60) {
    ++blink_count_;
  } else {
    blink_count_ = 0;
  }
}
