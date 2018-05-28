#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void init_fighter() {
  Fighter.pos.x = 280;
  Fighter.pos.y = 400;
  Fighter.shot_timer = 0; /* 0: can shot */
  for (int i = 0; i < FIGHTER_SHOT_MAX; ++i) {
    Fighter_shot[i].view = false;
  }
}

void move_fighter(InputManager &input_manager, MixerManager &mixer_manager) {
  double move_speed = 4.0;

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

  if (Fighter.shot_timer == 0) {
    if (input_manager.press_key_p(input_device::f)) {
      double shot_speed = 16;
      Vector pos = {25, 10};
      for (int i = 0; i < FIGHTER_SHOT_MAX; ++i) {
        if (Fighter_shot[i].view) {
          continue;
        }
        Fighter_shot[i].view = true;
        add_vec(Fighter_shot[i].pos, Fighter.pos, pos);
        Fighter_shot[i].move.x = 0;
        Fighter_shot[i].move.y = -shot_speed;
        Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_shoot), 0);
        break;
      }
      Fighter.shot_timer = 8;
    }
  } else {
    --Fighter.shot_timer;
    if (!input_manager.press_key_p(input_device::f)) {
      Fighter.shot_timer = 0;
    }
  }
}

void move_fighter_shot() {
  for (int i = 0; i < FIGHTER_SHOT_MAX; ++i) {
    if (!Fighter_shot[i].view) {
      continue;
    }
    add_vec(Fighter_shot[i].pos, Fighter_shot[i].move);
    if (Fighter_shot[i].pos.y < -16) {
      Fighter_shot[i].view = false;
    }
  }
}

bool check_enemyshots_hit_mychara() {
  SDL_Rect r1 = {Fighter.pos.x + 20, Fighter.pos.y + 16, 20, 22};

  if (Enemy_select == ENEMY_1) {
    for (int i = 0; i < ENEMY_SHOT_MAX; ++i) {
      if (!Enemy_shot[i].view) {
        continue;
      }
      SDL_Rect r2 = {Enemy_shot[i].pos.x + 6, Enemy_shot[i].pos.y + 6, 4, 4};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }
      --Chara_life;
      Enemy_shot[i].view = false;
      for (int j = 0; j < EFFECT_MAX; ++j) {
        if (Effect[j].view) {
          continue;
        }
        Effect[j].view = true;
        Effect[j].pos.x = -80 + Fighter.pos.x + 30;
        Effect[j].pos.y = -80 + Fighter.pos.y + 30;
        Effect[j].count = 0;
        break;
      }
    }
  } else if (Enemy_select == BOSS_1) {
    for (int i = 0; i < BOSS_SHOT_MAX; ++i) {
      if (!Boss_shot[i].view) {
        continue;
      }
      SDL_Rect r2 = {Boss_shot[i].pos.x + 3, Boss_shot[i].pos.y + 3, 10, 10};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }
      --Chara_life;
      Boss_shot[i].view = false;
      for (int j = 0; j < EFFECT_MAX; ++j) {
        if (Effect[j].view) {
          continue;
        }
        Effect[j].view = true;
        Effect[j].pos.x = -80 + Fighter.pos.x + 30;
        Effect[j].pos.y = -80 + Fighter.pos.y + 30;
        Effect[j].count = 0;
        break;
      }
    }
  }
  return Chara_life <= 0;
}

void draw_fighter(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::fighter);
  SDL_Rect dst = {Fighter.pos.x, Fighter.pos.y};
  SDL_BlitSurface(p_surface, NULL, screen, &dst);
}

void draw_fighter_shot(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::oval_re);

  for (int i = 0; i < FIGHTER_SHOT_MAX; ++i) {
    if (!Fighter_shot[i].view) {
      continue;
    }
    SDL_Rect dst = {Fighter_shot[i].pos.x, Fighter_shot[i].pos.y};
    SDL_BlitSurface(p_surface, NULL, screen, &dst);
  }
}
