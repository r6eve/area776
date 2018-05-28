#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "point.hpp"
#include "util.hpp"

void init_enemy() {
  for (auto &enemy : Enemy) {
    enemy.view = false;
  }
  for (auto &shot : Enemy_shot) {
    shot.view = false;
  }
}

void appear_enemy() {
  const double speed = 8;
  if (rand() % 45 != 0) {
    return;
  }

  for (auto &enemy : Enemy) {
    if (enemy.view) {
      continue;
    }
    enemy.view = true;
    enemy.pos.x = rand() % (screen::width - 64);
    enemy.pos.y = -64;
    enemy.shot_timer = rand() % 15 + 15;
    enemy.move.sub(Fighter.pos, enemy.pos);
    enemy.move.norm();
    enemy.move.mul(speed);
    break;
  }
}

void move_enemy(MixerManager &mixer_manager) {
  const double speed = 6;
  const double shot_pitch = 20;

  for (auto &enemy : Enemy) {
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
    Point fighter_center = {Fighter.pos.x + 32, Fighter.pos.y + 32};
    Point p;
    p.sub(fighter_center, enemy_center);
    p.norm();
    p.mul(speed);
    /* 時計回りに(shot_pitch * 2)度回転させておく */
    const double rot_angle = -(shot_pitch * 2) * M_PI / 180;
    p.rot(rot_angle);
    for (int _ = 0; _ < 5; ++_) {
      for (auto &shot : Enemy_shot) {
        if (shot.view) {
          continue;
        }

        shot.view = true;
        shot.pos.copy(enemy_center);
        shot.move.copy(p);
        break;
      }
      const double rot_angle = shot_pitch * M_PI / 180;
      p.rot(rot_angle);
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
    }
  }
}

void move_enemy_shot() {
  for (auto &shot : Enemy_shot) {
    if (!shot.view) {
      continue;
    }

    shot.pos.add(shot.move);
    if (shot.pos.x < -16) {
      shot.view = false;
    }
    if (shot.pos.y < -16) {
      shot.view = false;
    }
    if (shot.pos.x > screen::width) {
      shot.view = false;
    }
    if (shot.pos.y > screen::height) {
      shot.view = false;
    }
  }
}

bool check_myshots_hit_enemy() {
  for (auto &enemy : Enemy) {
    if (!enemy.view) {
      continue;
    }

    for (auto &shot : Fighter_shot) {
      if (!shot.view) {
        continue;
      }

      SDL_Rect r1 = {static_cast<Sint16>(enemy.pos.x),
                     static_cast<Sint16>(enemy.pos.y), 35, 35};
      SDL_Rect r2 = {static_cast<Sint16>(shot.pos.x),
                     static_cast<Sint16>(shot.pos.y), 10, 24};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      ++Enemy_life;
      enemy.view = false;
      shot.view = false;
      for (auto &effect : Effect) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + enemy.pos.x + r1.w / 2;
        effect.pos.y = -80 + enemy.pos.y + r1.h / 2;
        effect.count = 0;
        break;
      }
      if (Enemy_life > 29) {
        return true;
      }

      break;
    }
  }
  return false;
}

void draw_enemy(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &enemy : Enemy) {
    if (!enemy.view) {
      continue;
    }

    SDL_Surface *p_surface = image_manager.get(image::mons13);
    SDL_Rect dst = {static_cast<Sint16>(enemy.pos.x),
                    static_cast<Sint16>(enemy.pos.y), 35, 35};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}

void draw_enemy_shot(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &shot : Enemy_shot) {
    if (!shot.view) {
      continue;
    }

    SDL_Surface *p_surface = image_manager.get(image::bm01);
    SDL_Rect dst = {static_cast<Sint16>(shot.pos.x),
                    static_cast<Sint16>(shot.pos.y), 16, 16};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}
