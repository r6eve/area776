#include "boss.hpp"
#include <SDL/SDL_mixer.h>
#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

void Boss::update(const MixerManager &mixer_manager) noexcept {
  pos_.x += move_;
  if (pos_.x < 0) {
    pos_.x = 0;
    move_ = 2;
  }
  if ((pos_.x + 400) >= screen::width) {
    pos_.x = screen::width - 400;
    move_ = -2;
  }

  switch (state_) {
    case boss_state::automove: {
      if (pos_.y < 10) {
        pos_.y += 4;
      } else {
        shot_count_ = 0;
        shot_rot_ = 0;
        state_ = boss_state::attack00;
        move_ = -2;
      }
      break;
    }
    case boss_state::attack00: {
      if (!(shot_count_ % 2)) {
        const Point p[2] = {{88, 120}, {418 - 88, 120}};
        for (int n = 0; n < 2; ++n) {
          for (auto &bullet : bullets) {
            if (bullet.view) {
              continue;
            }
            bullet.set_pos(pos_ + p[n]);
            double r = PI * shot_rot_ / 10;
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
        ++shot_rot_;
      }
      ++shot_count_;
      if (shot_count_ == 240) {
        state_ = boss_state::attack01;
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
        bullets[i].set_pos(pos_ + Point{418 / 2, 105});
        double r = PI * i / 24;
        bullets[i].move.rot(Point{0, 3}, r);
        bullets[i].view = true;
        bullets[i].count = 0;
        bullets[i].rot = 0;
      }
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_shoot), 0);
      shot_count_ = 0;
      shot_rot_ = 0;
      if (state_ == boss_state::attack01) {
        state_ = boss_state::attack02;
      } else {
        state_ = boss_state::attack00;
      }
      break;
    }
  }
}
