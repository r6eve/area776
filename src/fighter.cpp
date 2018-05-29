#include "fighter.hpp"
#include <SDL/SDL_mixer.h>
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void Fighter::init() {
  pos.x = 280;
  pos.y = 400;
  shot_timer = 0;
  for (auto &bullet : bullets) {
    bullet.view = false;
  }
}

void Fighter::update(InputManager &input_manager, MixerManager &mixer_manager) {
  const double move_speed = 4.0;

  if (input_manager.press_key_p(input_device::up)) {
    pos.y -= move_speed;
  }
  if (input_manager.press_key_p(input_device::down)) {
    pos.y += move_speed;
  }
  if (input_manager.press_key_p(input_device::left)) {
    pos.x -= move_speed;
  }
  if (input_manager.press_key_p(input_device::right)) {
    pos.x += move_speed;
  }

  if (pos.x < 0) {
    pos.x = 0;
  }
  if (pos.y < 0) {
    pos.y = 0;
  }
  if (pos.x > (screen::width - 60)) {
    pos.x = screen::width - 60;
  }
  if (pos.y > (screen::height - 70)) {
    pos.y = screen::height - 70;
  }

  if (shot_timer != 0) {
    --shot_timer;
    return;
  }

  if (!input_manager.press_key_p(input_device::f)) {
    return;
  }

  double shot_speed = 16;
  Point p = {25, 10};
  for (auto &bullet : bullets) {
    if (bullet.view) {
      continue;
    }

    bullet.view = true;
    bullet.pos.add(pos, p);
    bullet.move.x = 0;
    bullet.move.y = -shot_speed;
    Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
    break;
  }
  shot_timer = 8;
}

void Fighter::update_shot() {
  for (auto &bullet : bullets) {
    if (!bullet.view) {
      continue;
    }

    bullet.pos.add(bullet.move);
    if (bullet.pos.y < -16) {
      bullet.view = false;
    }
  }
}

bool Fighter::check_enemyshots_hit_mychara(Enemy &enemy, Boss &boss,
                                           Effect &effect) {
  SDL_Rect r1 = {static_cast<Sint16>(pos.x + 20),
                 static_cast<Sint16>(pos.y + 16), 20, 22};

  if (Enemy_select == enemy_type::enemy) {
    for (auto &bullet : enemy.bullets) {
      if (!bullet.view) {
        continue;
      }

      SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x + 6),
                     static_cast<Sint16>(bullet.pos.y + 6), 4, 4};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      --life;
      bullet.view = false;
      for (auto &effect : effect.effects) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + pos.x + 30;
        effect.pos.y = -80 + pos.y + 30;
        effect.count = 0;
        break;
      }
    }
  } else if (Enemy_select == enemy_type::boss) {
    for (auto &bullet : boss.bullets) {
      if (!bullet.view) {
        continue;
      }

      SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x + 3),
                     static_cast<Sint16>(bullet.pos.y + 3), 10, 10};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      --life;
      bullet.view = false;
      for (auto &effect : effect.effects) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + pos.x + 30;
        effect.pos.y = -80 + pos.y + 30;
        effect.count = 0;
        break;
      }
    }
  }

  return life <= 0;
}

void Fighter::draw(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::fighter);
  SDL_Rect dst = {static_cast<Sint16>(pos.x), static_cast<Sint16>(pos.y), 60,
                  60};
  SDL_BlitSurface(p_surface, nullptr, screen, &dst);
}

void Fighter::draw_shot(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::oval_re);
  for (auto &bullet : bullets) {
    if (!bullet.view) {
      continue;
    }

    SDL_Rect dst = {static_cast<Sint16>(bullet.pos.x),
                    static_cast<Sint16>(bullet.pos.y), 10, 24};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}
