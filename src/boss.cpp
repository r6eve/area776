#include "boss.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

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
            double r = PI * shot_rot / 10;
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
        double r = PI * i / 24;
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
