#include "boss.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "util.hpp"

void Boss::init() {
  state = boss_state::automove;
  x = (screen::width - 418) / 2;
  y = -240;
  move = 0;
  for (auto &bullet : bullets) {
    bullet.view = false;
  }
}

void Boss::update(MixerManager &mixer_manager) {
  x += move;
  if (x < 0) {
    x = 0;
    move = 2;
  }
  if ((x + 400) >= screen::width) {
    x = screen::width - 400;
    move = -2;
  }

  switch (state) {
    case boss_state::automove: {
      if (y < 10) {
        y += 4;
      } else {
        shot_count = 0;
        shot_rot = 0;
        state = boss_state::attack00;
        move = -2;
      }
      break;
    }
    case boss_state::attack00: {
      if (!(shot_count % 2)) {
        int Pos[2][2] = {{88, 120}, {418 - 88, 120}};
        for (int n = 0; n < 2; ++n) {
          for (auto &bullet : bullets) {
            if (bullet.view) {
              continue;
            }
            bullet.pos.x = x + Pos[n][0];
            bullet.pos.y = y + Pos[n][1];
            double r = M_PI * shot_rot / 10;
            if (n == 1) {
              r = -r;
            }
            Point p = {0, 4};
            bullet.move.rot(p, r);
            bullet.view = true;
            bullet.count = 0;
            bullet.rot = 0;
            break;
          }
        }
        ++shot_rot;
      }
      ++shot_count;
      if (shot_count == 240) {
        state = boss_state::attack01;
      }
      break;
    }
    case boss_state::attack01:
    case boss_state::attack02: {
      for (auto &bullet : bullets) {
        if (bullet.view) {
          return;
        }
      }
      for (int i = 0; i < 48; ++i) {
        bullets[i].pos.x = x + 418 / 2;
        bullets[i].pos.y = y + 105;
        double r = M_PI * i / 24;
        Point p = {0, 3};
        bullets[i].move.rot(p, r);
        bullets[i].view = true;
        bullets[i].count = 0;
        bullets[i].rot = 0;
      }
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
      shot_count = 0;
      shot_rot = 0;
      if (state == boss_state::attack01) {
        state = boss_state::attack02;
      } else {
        state = boss_state::attack00;
      }
      break;
    }
  }
}

void Boss::update_shot() {
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
    ++bullet.count;
    bullet.count %= 2;
    if (bullet.count == 0) {
      ++bullet.rot;
      bullet.rot %= 16;
    }
  }
}

bool Boss::check_myshots_hit_boss() {
  for (auto &bullet : Fighter.bullets) {
    if (!bullet.view) {
      continue;
    }
    SDL_Rect r1 = {static_cast<Sint16>(bullet.pos.x),
                   static_cast<Sint16>(bullet.pos.y), 10, 24};
    SDL_Rect r2 = {static_cast<Sint16>(x + 171), static_cast<Sint16>(y + 95),
                   57, 57};
    if (!check_hit_rect(&r1, &r2)) {
      continue;
    }
    ++life;
    bullet.view = false;
    for (auto &effect : Effect) {
      if (effect.view) {
        continue;
      }
      effect.view = true;
      effect.pos.x = -80 + bullet.pos.x + r1.w / 2;
      effect.pos.y = -80 + bullet.pos.y + r1.h / 2;
      effect.count = 0;
      break;
    }
    if (life > 99) {
      return true;
    }
    break;
  }
  return false;
}

void Boss::draw(SDL_Surface *screen, ImageManager &image_manager) {
  SDL_Surface *p_surface = image_manager.get(image::boss);
  SDL_Rect dst = {static_cast<Sint16>(x), static_cast<Sint16>(y), 400, 224};
  SDL_BlitSurface(p_surface, nullptr, screen, &dst);
}

void Boss::draw_shot(SDL_Surface *screen, ImageManager &image_manager) {
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
