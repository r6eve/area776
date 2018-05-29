#include "fighter.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "point.hpp"
#include "util.hpp"

void FighterClass::init() {
  Fighter.pos.x = 280;
  Fighter.pos.y = 400;
  Fighter.shot_timer = 0;
  for (auto &shot : Fighter_shot) {
    shot.view = false;
  }
}

void FighterClass::move(InputManager &input_manager,
                        MixerManager &mixer_manager) {
  const double move_speed = 4.0;

  if (input_manager.press_key_p(input_device::up)) {
    Fighter.pos.y -= move_speed;
  }
  if (input_manager.press_key_p(input_device::down)) {
    Fighter.pos.y += move_speed;
  }
  if (input_manager.press_key_p(input_device::left)) {
    Fighter.pos.x -= move_speed;
  }
  if (input_manager.press_key_p(input_device::right)) {
    Fighter.pos.x += move_speed;
  }

  if (Fighter.pos.x < 0) {
    Fighter.pos.x = 0;
  }
  if (Fighter.pos.y < 0) {
    Fighter.pos.y = 0;
  }
  if (Fighter.pos.x > (screen::width - 60)) {
    Fighter.pos.x = screen::width - 60;
  }
  if (Fighter.pos.y > (screen::height - 70)) {
    Fighter.pos.y = screen::height - 70;
  }

  if (Fighter.shot_timer != 0) {
    --Fighter.shot_timer;
    return;
  }

  if (!input_manager.press_key_p(input_device::f)) {
    return;
  }

  double shot_speed = 16;
  Point pos = {25, 10};
  for (auto &shot : Fighter_shot) {
    if (shot.view) {
      continue;
    }

    shot.view = true;
    shot.pos.add(Fighter.pos, pos);
    shot.move.x = 0;
    shot.move.y = -shot_speed;
    Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
    break;
  }
  Fighter.shot_timer = 8;
}

void FighterClass::move_shot() {
  for (auto &shot : Fighter_shot) {
    if (!shot.view) {
      continue;
    }

    shot.pos.add(shot.move);
    if (shot.pos.y < -16) {
      shot.view = false;
    }
  }
}

bool FighterClass::check_enemyshots_hit_mychara() {
  SDL_Rect r1 = {static_cast<Sint16>(Fighter.pos.x + 20),
                 static_cast<Sint16>(Fighter.pos.y + 16), 20, 22};

  if (Enemy_select == ENEMY_1) {
    for (auto &shot : Enemy_shot) {
      if (!shot.view) {
        continue;
      }

      SDL_Rect r2 = {static_cast<Sint16>(shot.pos.x + 6),
                     static_cast<Sint16>(shot.pos.y + 6), 4, 4};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      --Fighter.life;
      shot.view = false;
      for (auto &effect : Effect) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + Fighter.pos.x + 30;
        effect.pos.y = -80 + Fighter.pos.y + 30;
        effect.count = 0;
        break;
      }
    }
  } else if (Enemy_select == BOSS_1) {
    for (auto &shot : Boss_shot) {
      if (!shot.view) {
        continue;
      }

      SDL_Rect r2 = {static_cast<Sint16>(shot.pos.x + 3),
                     static_cast<Sint16>(shot.pos.y + 3), 10, 10};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      --Fighter.life;
      shot.view = false;
      for (auto &effect : Effect) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + Fighter.pos.x + 30;
        effect.pos.y = -80 + Fighter.pos.y + 30;
        effect.count = 0;
        break;
      }
    }
  }

  return Fighter.life <= 0;
}

void FighterClass::draw(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::fighter);
  SDL_Rect dst = {static_cast<Sint16>(Fighter.pos.x),
                  static_cast<Sint16>(Fighter.pos.y), 60, 60};
  SDL_BlitSurface(p_surface, nullptr, screen, &dst);
}

void FighterClass::draw_shot(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::oval_re);
  for (auto &shot : Fighter_shot) {
    if (!shot.view) {
      continue;
    }

    SDL_Rect dst = {static_cast<Sint16>(shot.pos.x),
                    static_cast<Sint16>(shot.pos.y), 10, 24};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}
