#include "enemy.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "effect.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

void Enemy::update(const MixerManager &mixer_manager,
                   const Fighter &fighter) noexcept {
  const double speed = 6;
  const double shot_pitch = 20;

  for (auto &enemy : enemies) {
    if (!enemy.view) {
      continue;
    }

    enemy.pos += enemy.move;
    if (enemy.pos.x < -35) {
      enemy.view = false;
      continue;
    }
    if (enemy.pos.x > screen::width) {
      enemy.view = false;
      continue;
    }
    if (enemy.pos.y > screen::height) {
      enemy.view = false;
      continue;
    }

    --enemy.shot_timer;
    if (enemy.shot_timer != 0) {
      continue;
    }

    const Point enemy_center = enemy.pos + 32;
    const Point fighter_center = fighter.get_pos() + 32;
    Point p = fighter_center - enemy_center;
    p.norm();
    p *= speed;
    /* 時計回りに(shot_pitch * 2)度回転させておく */
    const double rot_angle = -(shot_pitch * 2) * PI / 180;
    p.rot(rot_angle);
    for (int _ = 0; _ < 5; ++_) {
      for (auto &bullet : bullets) {
        if (bullet.view) {
          continue;
        }

        bullet.view = true;
        bullet.pos = enemy_center;
        bullet.move = p;
        break;
      }
      const double rot_angle = shot_pitch * PI / 180;
      p.rot(rot_angle);
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
    }
  }
}
