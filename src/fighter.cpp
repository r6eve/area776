#include "fighter.hpp"
#include <SDL/SDL_mixer.h>
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"

void Fighter::update(InputManager &input_manager,
                     MixerManager &mixer_manager) noexcept {
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

  const double shot_speed = 16;
  for (auto &bullet : bullets) {
    if (bullet.view) {
      continue;
    }

    bullet.view = true;
    bullet.pos = pos + Point{25, 10};
    bullet.move = Point{0, -shot_speed};
    Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
    break;
  }
  shot_timer = 8;
}
