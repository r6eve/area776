#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void init_boss() {
  Boss.state = BOSS_STATE_AUTOMOVE;
  Boss.x = (SCREEN_WIDTH - 418) / 2;
  Boss.y = -240;
  Boss.move = 0;
  for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
    Boss_shot[i].view = false;
  }
}

void move_boss(MixerManager &mixer_manager) {
  Boss.x += Boss.move;
  if (Boss.x < 0) {
    Boss.x = 0;
    Boss.move = 2;
  }
  if ((Boss.x + 400) >= SCREEN_WIDTH) {
    Boss.x = SCREEN_WIDTH - 400;
    Boss.move = -2;
  }

  switch (Boss.state) {
    case BOSS_STATE_AUTOMOVE: {
      if (Boss.y < 10) {
        Boss.y += 4;
      } else {
        Boss.shot_count = 0;
        Boss.shot_rot = 0;
        Boss.state = BOSS_STATE_ATTACK_00;
        Boss.move = -2;
      }
      break;
    }
    case BOSS_STATE_ATTACK_00: {
      if (!(Boss.shot_count % 2)) {
        int Pos[2][2] = {{88, 120}, {418 - 88, 120}};
        for (int n = 0; n < 2; ++n) {
          for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
            if (Boss_shot[i].view) {
              continue;
            }
            Boss_shot[i].pos.x = Boss.x + Pos[n][0];
            Boss_shot[i].pos.y = Boss.y + Pos[n][1];
            double r = M_PI * Boss.shot_rot / 10;
            if (n == 1) {
              r = -r;
            }
            Vector vec = {0, 4};
            rot_vec(&Boss_shot[i].move, &vec, r);
            Boss_shot[i].view = true;
            Boss_shot[i].count = 0;
            Boss_shot[i].rot = 0;
            break;
          }
        }
        ++Boss.shot_rot;
      }
      ++Boss.shot_count;
      if (Boss.shot_count == 240) {
        Boss.state = BOSS_STATE_ATTACK_01;
      }
      break;
    }
    case BOSS_STATE_ATTACK_01:
    case BOSS_STATE_ATTACK_02: {
      int view = false;
      for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
        if (Boss_shot[i].view) {
          view = true;
          break;
        }
      }
      if (view) {
        break;
      }
      for (int i = 0; i < 48; ++i) {
        Boss_shot[i].pos.x = Boss.x + 418 / 2;
        Boss_shot[i].pos.y = Boss.y + 105;
        double r = M_PI * i / 24;
        Vector vec = {0, 3};
        rot_vec(&Boss_shot[i].move, &vec, r);
        Boss_shot[i].view = true;
        Boss_shot[i].count = 0;
        Boss_shot[i].rot = 0;
      }
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
      Boss.shot_count = 0;
      Boss.shot_rot = 0;
      if (Boss.state == BOSS_STATE_ATTACK_01) {
        Boss.state = BOSS_STATE_ATTACK_02;
      } else {
        Boss.state = BOSS_STATE_ATTACK_00;
      }
      break;
    }
  }
}

void move_boss_shot() {
  for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
    if (!Boss_shot[i].view) {
      continue;
    }
    add_vec(&Boss_shot[i].pos, &Boss_shot[i].move);
    if (Boss_shot[i].pos.x < -16) {
      Boss_shot[i].view = false;
    }
    if (Boss_shot[i].pos.y < -16) {
      Boss_shot[i].view = false;
    }
    if (Boss_shot[i].pos.x > SCREEN_WIDTH) {
      Boss_shot[i].view = false;
    }
    if (Boss_shot[i].pos.y > SCREEN_HEIGHT) {
      Boss_shot[i].view = false;
    }
    ++Boss_shot[i].count;
    Boss_shot[i].count %= 2;
    if (Boss_shot[i].count == 0) {
      ++Boss_shot[i].rot;
      Boss_shot[i].rot %= 16;
    }
  }
}

void check_myshots_hit_boss() {
  for (int i = 0; i < FIGHTER_SHOT_MAX; ++i) {
    if (!Fighter_shot[i].view) {
      continue;
    }
    SDL_Rect r1 = {Fighter_shot[i].pos.x, Fighter_shot[i].pos.y, 10, 24};
    SDL_Rect r2 = {Boss.x + 171, Boss.y + 95, 57, 57};
    if (!check_hit_rect(&r1, &r2)) {
      continue;
    }
    ++Boss_life;
    Fighter_shot[i].view = false;
    for (int j = 0; j < EFFECT_MAX; ++j) {
      if (Effect[j].view) {
        continue;
      }
      Effect[j].view = true;
      Effect[j].pos.x = -80 + Fighter_shot[i].pos.x + r1.w / 2;
      Effect[j].pos.y = -80 + Fighter_shot[i].pos.y + r1.h / 2;
      Effect[j].count = 0;
      break;
    }
    if (Boss_life > 99) {
      Game_state = GAME_STATE_CLEAR;
      Game_count = 0;
    }
    break;
  }
}

void draw_boss() {
  SDL_Surface *p_surface = get_img("boss");
  SDL_Rect dst = {Boss.x, Boss.y};
  SDL_BlitSurface(p_surface, NULL, Screen, &dst);
}

void draw_boss_shot() {
  for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
    if (!Boss_shot[i].view) {
      continue;
    }
    SDL_Surface *p_surface = get_img("bm01");
    SDL_Rect dst = {Boss_shot[i].pos.x, Boss_shot[i].pos.y};
    SDL_BlitSurface(p_surface, NULL, Screen, &dst);
  }
}
