#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void init_boss() {
  Boss.state = boss_state::automove;
  Boss.x = (screen::width - 418) / 2;
  Boss.y = -240;
  Boss.move = 0;
  for (auto &shot : Boss_shot) {
    shot.view = false;
  }
}

void move_boss(MixerManager &mixer_manager) {
  Boss.x += Boss.move;
  if (Boss.x < 0) {
    Boss.x = 0;
    Boss.move = 2;
  }
  if ((Boss.x + 400) >= screen::width) {
    Boss.x = screen::width - 400;
    Boss.move = -2;
  }

  switch (Boss.state) {
    case boss_state::automove: {
      if (Boss.y < 10) {
        Boss.y += 4;
      } else {
        Boss.shot_count = 0;
        Boss.shot_rot = 0;
        Boss.state = boss_state::attack00;
        Boss.move = -2;
      }
      break;
    }
    case boss_state::attack00: {
      if (!(Boss.shot_count % 2)) {
        int Pos[2][2] = {{88, 120}, {418 - 88, 120}};
        for (int n = 0; n < 2; ++n) {
          for (auto &shot : Boss_shot) {
            if (shot.view) {
              continue;
            }
            shot.pos.x = Boss.x + Pos[n][0];
            shot.pos.y = Boss.y + Pos[n][1];
            double r = M_PI * Boss.shot_rot / 10;
            if (n == 1) {
              r = -r;
            }
            Vector vec = {0, 4};
            rot_vec(shot.move, vec, r);
            shot.view = true;
            shot.count = 0;
            shot.rot = 0;
            break;
          }
        }
        ++Boss.shot_rot;
      }
      ++Boss.shot_count;
      if (Boss.shot_count == 240) {
        Boss.state = boss_state::attack01;
      }
      break;
    }
    case boss_state::attack01:
    case boss_state::attack02: {
      for (auto &shot : Boss_shot) {
        if (shot.view) {
          return;
        }
      }
      for (int i = 0; i < 48; ++i) {
        Boss_shot[i].pos.x = Boss.x + 418 / 2;
        Boss_shot[i].pos.y = Boss.y + 105;
        double r = M_PI * i / 24;
        Vector vec = {0, 3};
        rot_vec(Boss_shot[i].move, vec, r);
        Boss_shot[i].view = true;
        Boss_shot[i].count = 0;
        Boss_shot[i].rot = 0;
      }
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
      Boss.shot_count = 0;
      Boss.shot_rot = 0;
      if (Boss.state == boss_state::attack01) {
        Boss.state = boss_state::attack02;
      } else {
        Boss.state = boss_state::attack00;
      }
      break;
    }
  }
}

void move_boss_shot() {
  for (auto &shot : Boss_shot) {
    if (!shot.view) {
      continue;
    }
    add_vec(shot.pos, shot.move);
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
    ++shot.count;
    shot.count %= 2;
    if (shot.count == 0) {
      ++shot.rot;
      shot.rot %= 16;
    }
  }
}

bool check_myshots_hit_boss() {
  for (auto &shot : Fighter_shot) {
    if (!shot.view) {
      continue;
    }
    SDL_Rect r1 = {static_cast<Sint16>(shot.pos.x),
                   static_cast<Sint16>(shot.pos.y), 10, 24};
    SDL_Rect r2 = {static_cast<Sint16>(Boss.x + 171),
                   static_cast<Sint16>(Boss.y + 95), 57, 57};
    if (!check_hit_rect(&r1, &r2)) {
      continue;
    }
    ++Boss_life;
    shot.view = false;
    for (auto &effect : Effect) {
      if (effect.view) {
        continue;
      }
      effect.view = true;
      effect.pos.x = -80 + shot.pos.x + r1.w / 2;
      effect.pos.y = -80 + shot.pos.y + r1.h / 2;
      effect.count = 0;
      break;
    }
    if (Boss_life > 99) {
      return true;
    }
    break;
  }
  return false;
}

void draw_boss(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::boss);
  SDL_Rect dst = {static_cast<Sint16>(Boss.x), static_cast<Sint16>(Boss.y), 400,
                  224};
  SDL_BlitSurface(p_surface, nullptr, screen, &dst);
}

void draw_boss_shot(SDL_Surface *screen, ImageManager &image_manager) {
  for (auto &shot : Boss_shot) {
    if (!shot.view) {
      continue;
    }
    SDL_Surface *p_surface = image_manager.get(image::bm01);
    SDL_Rect dst = {static_cast<Sint16>(shot.pos.x),
                    static_cast<Sint16>(shot.pos.y), 16, 16};
    SDL_BlitSurface(p_surface, nullptr, screen, &dst);
  }
}
