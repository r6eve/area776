#include "boss.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

void Boss::update(MixerManager &mixer_manager) noexcept {
  pos.x += move;
  if (pos.x < 0) {
    pos.x = 0;
    move = 2;
  }
  if ((pos.x + 400) >= screen::width) {
    pos.x = screen::width - 400;
    move = -2;
  }

  switch (state) {
    case boss_state::automove: {
      if (pos.y < 10) {
        pos.y += 4;
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
        const Point p[2] = {{88, 120}, {418 - 88, 120}};
        for (int n = 0; n < 2; ++n) {
          for (auto &bullet : bullets) {
            if (bullet.view) {
              continue;
            }
            bullet.pos = pos + p[n];
            double r = PI * shot_rot / 10;
            if (n == 1) {
              r = -r;
            }
            bullet.move.rot(Point{0, 4}, r);
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
        bullets[i].pos = pos + Point{418 / 2, 105};
        double r = PI * i / 24;
        bullets[i].move.rot(Point{0, 3}, r);
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
