#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "util.hpp"
#include "mixer_manager.hpp"

void init_enemy() {
  for (int i = 0; i < ENEMY_MAX; ++i) {
    Enemy[i].view = false;
  }
  for (int i = 0; i < ENEMY_SHOT_MAX; ++i) {
    Enemy_shot[i].view = false;
  }
}

void appear_enemy() {
  double speed = 8;
  int r = rand() % 45;

  if (r == 0) {
    for (int i = 0; i < ENEMY_MAX; ++i) {
      if (Enemy[i].view) {
        continue;
      }
      Enemy[i].view = true;
      Enemy[i].pos.x = rand() % (screen::width - 64);
      Enemy[i].pos.y = -64;
      Enemy[i].shot_timer = rand() % 15 + 15;
      sub_vec(&Enemy[i].move, &Fighter.pos, &Enemy[i].pos);
      normalize_vec(&Enemy[i].move);
      mul_vec(&Enemy[i].move, speed);
      break;
    }
  }
}

void move_enemy(MixerManager &mixer_manager) {
  double speed = 6;
  double shot_pitch = 20;

  for (int i = 0; i < ENEMY_MAX; ++i) {
    if (!Enemy[i].view) {
      continue;
    }
    add_vec(&Enemy[i].pos, &Enemy[i].move);
    if (Enemy[i].pos.x < -35) {
      Enemy[i].view = 0;
    }
    if (Enemy[i].pos.x > screen::width) {
      Enemy[i].view = 0;
    }
    if (Enemy[i].pos.y > screen::height) {
      Enemy[i].view = 0;
    }
    if (!Enemy[i].view) {
      continue;
    }
    --Enemy[i].shot_timer;
    if (Enemy[i].shot_timer == 0) {
      Vector enemy_center = {Enemy[i].pos.x + 32, Enemy[i].pos.y + 32};
      Vector fighter_center = {Fighter.pos.x + 32, Fighter.pos.y + 32};
      Vector vec;
      sub_vec(&vec, &fighter_center, &enemy_center);
      normalize_vec(&vec);
      mul_vec(&vec, speed);
      /* 時計回りに(shot_pitch * 2)度回転させておく */
      double rot_angle = -(shot_pitch * 2) * M_PI / 180;
      rot_vec(&vec, rot_angle);
      for (int j = 0; j < 5; ++j) {
        for (int k = 0; k < ENEMY_SHOT_MAX; ++k) {
          if (Enemy_shot[k].view) {
            continue;
          }
          Enemy_shot[k].view = true;
          cp_vec(&Enemy_shot[k].pos, &enemy_center);
          cp_vec(&Enemy_shot[k].move, &vec);
          break;
        }
        double rot_angle = shot_pitch * M_PI / 180;
        rot_vec(&vec, rot_angle);
        Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
      }
    }
  }
}

void move_enemy_shot() {
  for (int i = 0; i < ENEMY_SHOT_MAX; ++i) {
    if (!Enemy_shot[i].view) {
      continue;
    }
    add_vec(&Enemy_shot[i].pos, &Enemy_shot[i].move);
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

void check_myshots_hit_enemy() {
  for (int i = 0; i < ENEMY_MAX; ++i) {
    if (!Enemy[i].view) continue;
    for (int j = 0; j < FIGHTER_SHOT_MAX; ++j) {
      if (!Fighter_shot[j].view) {
        continue;
      }
      SDL_Rect r1 = {Enemy[i].pos.x, Enemy[i].pos.y, 35, 35};
      SDL_Rect r2 = {Fighter_shot[j].pos.x, Fighter_shot[j].pos.y, 10, 24};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }
      ++Enemy_life;
      Enemy[i].view = false;
      Fighter_shot[j].view = false;
      for (int k = 0; k < EFFECT_MAX; ++k) {
        if (Effect[k].view) {
          continue;
        }
        Effect[k].view = true;
        Effect[k].pos.x = -80 + Enemy[i].pos.x + r1.w / 2;
        Effect[k].pos.y = -80 + Enemy[i].pos.y + r1.h / 2;
        Effect[k].count = 0;
        break;
      }
      if (Enemy_life > 29) Enemy_select = BOSS_1;
      break;
    }
  }
}

void draw_enemy() {
  for (int i = 0; i < ENEMY_MAX; ++i) {
    if (!Enemy[i].view) {
      continue;
    }
    SDL_Surface *p_surface = get_img("mons13");
    SDL_Rect dst = {Enemy[i].pos.x, Enemy[i].pos.y};
    SDL_BlitSurface(p_surface, NULL, Screen, &dst);
  }
}

void draw_enemy_shot() {
  for (int i = 0; i < ENEMY_SHOT_MAX; ++i) {
    if (!Enemy_shot[i].view) {
      continue;
    }
    SDL_Surface *p_surface = get_img("bm01");
    SDL_Rect dst = {Enemy_shot[i].pos.x, Enemy_shot[i].pos.y};
    SDL_BlitSurface(p_surface, NULL, Screen, &dst);
  }
}
