//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#include "area776.hpp"
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "boss.hpp"
#include "def_global.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "input_manager.hpp"
#include "util.hpp"

void Area776::run() noexcept {
  for (;;) {
    input_manager_->update();
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
    }
    if (!poll_event()) {
      return;
    }
    if (debug_mode_) {
      draw_fps();
    }
    SDL_RenderPresent(renderer_);
    wait_game();
  }
}

void Area776::game_title() noexcept {
  SDL_SetRenderDrawColor(renderer_, 69, 64, 43, 255);
  SDL_RenderClear(renderer_);

  const Point title_pos = Point{160, 180};
  const Point message_pos = Point{210, 300};
  const char *title_str = "A  r  e  a  7  7  6";
  const char *message_str = "P r e s s  S p a c e  K e y";
  switch (game_count_) {
    case 0: {
      wipe_->set_wipe_in();
      ++game_count_;
      break;
    }
    case 1: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      wipe_->draw();
      if (wipe_->update()) {
        ++game_count_;
      }
      break;
    }
    case 2: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      ++blink_count_;
      if (blink_count_ >= 30) {
        SDL_SetRenderDrawColor(renderer_, 69, 64, 43, 255);
        const SDL_Rect dst = {static_cast<Sint16>(message_pos.x),
                              static_cast<Sint16>(message_pos.y),
                              screen::width - 240, screen::height - 300};
        SDL_RenderFillRect(renderer_, &dst);
        if (blink_count_ >= 60) {
          blink_count_ = 0;
        }
      }
      if (input_manager_->press_key_p(input_device::space)) {
        wipe_->set_wipe_out();
        ++game_count_;
      }
      break;
    }
    case 3: {
      draw_text(font_size::x36, rgb::dark_red, title_pos, title_str);
      draw_text(font_size::x16, rgb::black, message_pos, message_str);
      wipe_->draw();
      if (wipe_->update()) {
        Mix_PlayMusic(mixer_manager_->get_music(), -1);
        fighter_->init();
        enemies_->init(debug_mode_);
        effects_->init();
        snow_->init();
        boss_->init();
        game_count_ = 0;
        game_state_ = game_state::start;
        game_level_ = 1;
        enemy_select_ = enemy_type::enemy;
        fighter_->set_life(20);
        srand((unsigned int)time(nullptr));
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }
}

void Area776::game_start() noexcept {
  fighter_->update();
  draw_map();
  fighter_->draw();

  switch (game_count_) {
    case 0: {
      wipe_->set_wipe_in();
      wipe_->draw();
      ++game_count_;
      break;
    }
    case 1: {
      wipe_->draw();
      if (wipe_->update()) {
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

void Area776::play_game() noexcept {
  if (input_manager_->edge_key_p(input_device::space)) {
    game_state_ = game_state::pause;
  }

  fighter_->update();
  if (util::check_enemyshots_hit_fighter(enemy_select_, *fighter_, *enemies_,
                                         *boss_, *effects_, *mixer_manager_)) {
    game_state_ = game_state::gameover;
  }
  effects_->update();
  draw_map();

  switch (enemy_select_) {
    case enemy_type::enemy: {
      enemies_->update(*fighter_);
      if (util::check_fightershots_hit_enemy(*fighter_, *enemies_, *effects_,
                                             *mixer_manager_)) {
        enemy_select_ = enemy_type::boss;
      }
      snow_->update();
      snow_->draw();
      enemies_->draw();
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
        boss_->update();

        if (util::check_fightershots_hit_boss(*fighter_, *boss_, *effects_,
                                              *mixer_manager_)) {
          game_state_ = game_state::clear;
          game_count_ = 0;
        }

        boss_->draw();
      }
      break;
    }
  }

  fighter_->draw();
  effects_->draw();
  draw_life();
}

void Area776::game_clear() noexcept {
  fighter_->update();
  draw_map();
  fighter_->draw();
  draw_life();

  if (game_count_ == 0) {
    wipe_->set_wipe_out();
    wipe_->draw();
    ++game_count_;
    return;
  }

  wipe_->draw();
  if (!wipe_->update()) {
    return;
  }

  if (game_level_ == 1) {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    const SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_RenderFillRect(renderer_, &dst);
    draw_text(font_size::x36, rgb::red, Point{150, 180}, "G A M E  C L E A R");
    ++game_count_;
    if (game_count_ > 200) {
      wipe_->draw();
      if (wipe_->update()) {
        game_count_ = 0;
        game_state_ = game_state::title;
        Mix_HaltMusic();
      }
    }
    return;
  }

  fighter_->init();
  enemies_->init(debug_mode_);
  boss_->init();
  effects_->init();
  snow_->init();
  game_count_ = 0;
  game_state_ = game_state::start;
  ++game_level_;
}

void Area776::game_over() noexcept {
  draw_text(font_size::x36, rgb::red, Point{200, 180}, "G a m e O v e r");
  ++game_count_;
  if (game_count_ <= 200) {
    return;
  }

  wipe_->draw();
  if (!wipe_->update()) {
    return;
  }

  game_count_ = 0;
  game_state_ = game_state::title;
  Mix_HaltMusic();
}

void Area776::game_pause() noexcept {
  draw_map();

  switch (enemy_select_) {
    case enemy_type::enemy: {
      snow_->draw();
      enemies_->draw();
      break;
    }
    case enemy_type::boss: {
      boss_->draw();
      break;
    }
  }

  fighter_->draw();
  effects_->draw();
  draw_life();
  draw_translucence();
  if (input_manager_->edge_key_p(input_device::space)) {
    game_state_ = game_state::playing;
  }
}
