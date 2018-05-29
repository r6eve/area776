#include "fighter.hpp"
#include <SDL/SDL_mixer.h>
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"

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
