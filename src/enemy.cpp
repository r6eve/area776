#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
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
    sub_vec(enemy.move, Fighter.pos, enemy.pos);
    normalize_vec(enemy.move);
    mul_vec(enemy.move, speed);
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

    add_vec(enemy.pos, enemy.move);
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

    Vector enemy_center = {enemy.pos.x + 32, enemy.pos.y + 32};
    Vector fighter_center = {Fighter.pos.x + 32, Fighter.pos.y + 32};
    Vector vec;
    sub_vec(vec, fighter_center, enemy_center);
    normalize_vec(vec);
    mul_vec(vec, speed);
    /* 時計回りに(shot_pitch * 2)度回転させておく */
    const double rot_angle = -(shot_pitch * 2) * M_PI / 180;
    rot_vec(vec, rot_angle);
    for (int _ = 0; _ < 5; ++_) {
      for (auto &shot : Enemy_shot) {
        if (shot.view) {
          continue;
        }

        shot.view = true;
        cp_vec(shot.pos, enemy_center);
        cp_vec(shot.move, vec);
        break;
      }
      const double rot_angle = shot_pitch * M_PI / 180;
      rot_vec(vec, rot_angle);
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
    }
  }
}

void move_enemy_shot() {
  for (int i = 0; i < ENEMY_SHOT_MAX; ++i) {
    if (!Enemy_shot[i].view) {
      continue;
    }
    add_vec(Enemy_shot[i].pos, Enemy_shot[i].move);
    if (Enemy_shot[i].pos.x < -16) {
      Enemy_shot[i].view = false;
    }
    if (Enemy_shot[i].pos.y < -16) {
      Enemy_shot[i].view = false;
    }
    if (Enemy_shot[i].pos.x > screen::width) {
      Enemy_shot[i].view = false;
    }
    if (Enemy_shot[i].pos.y > screen::height) {
      Enemy_shot[i].view = false;
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
