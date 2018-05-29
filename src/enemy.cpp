#include "enemy.hpp"
#include "effect.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void Enemy::init() {
  for (auto &enemy : enemies) {
    enemy.view = false;
  }
  for (auto &bullet : bullets) {
    bullet.view = false;
  }
}

void Enemy::appear(Fighter &fighter) {
  const double speed = 8;
  if (rand() % 45 != 0) {
    return;
  }

  for (auto &enemy : enemies) {
    if (enemy.view) {
      continue;
    }
    enemy.view = true;
    enemy.pos.x = rand() % (screen::width - 64);
    enemy.pos.y = -64;
    enemy.shot_timer = rand() % 15 + 15;
    enemy.move.sub(fighter.pos, enemy.pos);
    enemy.move.norm();
    enemy.move.mul(speed);
    break;
  }
}

void Enemy::update(MixerManager &mixer_manager, Fighter &fighter) {
  const double speed = 6;
  const double shot_pitch = 20;

  for (auto &enemy : enemies) {
    if (!enemy.view) {
      continue;
    }

    enemy.pos.add(enemy.move);
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

    Point enemy_center = {enemy.pos.x + 32, enemy.pos.y + 32};
    Point fighter_center = {fighter.pos.x + 32, fighter.pos.y + 32};
    Point p;
    p.sub(fighter_center, enemy_center);
    p.norm();
    p.mul(speed);
    /* 時計回りに(shot_pitch * 2)度回転させておく */
    const double rot_angle = -(shot_pitch * 2) * M_PI / 180;
    p.rot(rot_angle);
    for (int _ = 0; _ < 5; ++_) {
      for (auto &bullet : bullets) {
        if (bullet.view) {
          continue;
        }

        bullet.view = true;
        bullet.pos.copy(enemy_center);
        bullet.move.copy(p);
        break;
      }
      const double rot_angle = shot_pitch * M_PI / 180;
      p.rot(rot_angle);
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
    }
  }
}

void Enemy::update_shot() {
  for (auto &bullet : bullets) {
    if (!bullet.view) {
      continue;
    }

    bullet.pos.add(bullet.move);
    if (bullet.pos.x < -16) {
      bullet.view = false;
    }
    if (bullet.pos.y < -16) {
      bullet.view = false;
    }
    if (bullet.pos.x > screen::width) {
      bullet.view = false;
    }
    if (bullet.pos.y > screen::height) {
      bullet.view = false;
    }
  }
}

bool Enemy::check_myshots_hit_enemy(Fighter &fighter, Effect &effect) {
  for (auto &enemy : enemies) {
    if (!enemy.view) {
      continue;
    }

    for (auto &bullet : fighter.bullets) {
      if (!bullet.view) {
        continue;
      }

      SDL_Rect r1 = {static_cast<Sint16>(enemy.pos.x),
                     static_cast<Sint16>(enemy.pos.y), 35, 35};
      SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x),
                     static_cast<Sint16>(bullet.pos.y), 10, 24};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      ++life;
      enemy.view = false;
      bullet.view = false;
      for (auto &effect : effect.effects) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + enemy.pos.x + r1.w / 2;
        effect.pos.y = -80 + enemy.pos.y + r1.h / 2;
        effect.count = 0;
        break;
      }
      if (life > 29) {
        return true;
      }

      break;
    }
  }
  return false;
}

void Enemy::draw(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &enemy : enemies) {
    if (!enemy.view) {
      continue;
    }

    SDL_Surface *p_surface = image_manager.get(image::mons13);
    SDL_Rect dst = {static_cast<Sint16>(enemy.pos.x),
                    static_cast<Sint16>(enemy.pos.y), 35, 35};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}

void Enemy::draw_shot(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &bullet : bullets) {
    if (!bullet.view) {
      continue;
    }

    SDL_Surface *p_surface = image_manager.get(image::bm01);
    SDL_Rect dst = {static_cast<Sint16>(bullet.pos.x),
                    static_cast<Sint16>(bullet.pos.y), 16, 16};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}
