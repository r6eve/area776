#include "util.hpp"
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "mixer_manager.hpp"

namespace util {

inline bool check_hit_rect(const SDL_Rect *a, const SDL_Rect *b) noexcept {
  return ((a->x) < (b->x + b->w)) && ((b->x) < (a->x + a->w)) &&
         ((a->y) < (b->y + b->h)) && ((b->y) < (a->y + a->h));
}

bool check_enemyshots_hit_fighter(const enemy_type enemy_select,
                                  Fighter &fighter, Enemy &enemy, Boss &boss,
                                  Effect &effect,
                                  const MixerManager &mixer_manager) noexcept {
  const Point fighter_pos = fighter.get_pos();
  SDL_Rect r1 = {static_cast<Sint16>(fighter_pos.x + 20),
                 static_cast<Sint16>(fighter_pos.y + 16), 20, 22};

  switch (enemy_select) {
    case enemy_type::enemy: {
      for (auto &bullet : enemy.bullets) {
        if (!bullet.view) {
          continue;
        }

        SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x + 6),
                       static_cast<Sint16>(bullet.pos.y + 6), 4, 4};
        if (!check_hit_rect(&r1, &r2)) {
          continue;
        }

        fighter.set_life(fighter.get_life() - 1);
        if (fighter.get_life() <= 0) {
          Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_down), 0);
        } else {
          Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_hit), 0);
        }
        bullet.view = false;
        for (auto &effect : effect.effects) {
          if (effect.view) {
            continue;
          }

          effect.view = true;
          effect.pos = fighter_pos - 50;
          effect.count = 0;
          break;
        }
      }
      break;
    }
    case enemy_type::boss: {
      for (auto &bullet : boss.bullets) {
        if (!bullet.view_p()) {
          continue;
        }

        const Point bullet_pos = bullet.get_pos();
        SDL_Rect r2 = {static_cast<Sint16>(bullet_pos.x + 3),
                       static_cast<Sint16>(bullet_pos.y + 3), 10, 10};
        if (!check_hit_rect(&r1, &r2)) {
          continue;
        }

        fighter.set_life(fighter.get_life() - 1);
        if (fighter.get_life() <= 0) {
          Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_down), 0);
        } else {
          Mix_PlayChannel(-1, mixer_manager.get_se(se_type::fighter_hit), 0);
        }
        bullet.make_invisible();
        for (auto &effect : effect.effects) {
          if (effect.view) {
            continue;
          }

          effect.view = true;
          effect.pos = fighter_pos - 50;
          effect.count = 0;
          break;
        }
      }
      break;
    }
    default:
      // NOTREACHED
      break;
  }

  return fighter.get_life() <= 0;
}

bool check_fightershots_hit_enemy(Fighter &fighter, Enemy &enemy,
                                  Effect &effect,
                                  const MixerManager &mixer_manager) noexcept {
  for (auto &e : enemy.enemies) {
    if (!e.view) {
      continue;
    }

    for (auto &bullet : fighter.bullets) {
      if (!bullet.view_p()) {
        continue;
      }

      const Point bullet_pos = bullet.get_pos();
      SDL_Rect r1 = {static_cast<Sint16>(e.pos.x), static_cast<Sint16>(e.pos.y),
                     35, 35};
      SDL_Rect r2 = {static_cast<Sint16>(bullet_pos.x),
                     static_cast<Sint16>(bullet_pos.y), 10, 24};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      enemy.set_life(enemy.get_life() - 1);
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_hit), 0);
      e.view = false;
      bullet.make_invisible();
      for (auto &effect : effect.effects) {
        if (effect.view) {
          continue;
        }

        effect.view = true;
        effect.pos.x = -80 + e.pos.x + r1.w / 2;
        effect.pos.y = -80 + e.pos.y + r1.h / 2;
        effect.count = 0;
        break;
      }
      if (enemy.get_life() <= 0) {
        return true;
      }

      break;
    }
  }
  return false;
}

bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss, Effect &effect,
                                 const MixerManager &mixer_manager) noexcept {
  const Point boss_pos = boss.get_pos();
  for (auto &bullet : fighter.bullets) {
    if (!bullet.view_p()) {
      continue;
    }
    const Point bullet_pos = bullet.get_pos();
    SDL_Rect r1 = {static_cast<Sint16>(bullet_pos.x),
                   static_cast<Sint16>(bullet_pos.y), 10, 24};
    SDL_Rect r2 = {static_cast<Sint16>(boss_pos.x + 171),
                   static_cast<Sint16>(boss_pos.y + 95), 57, 57};
    if (!check_hit_rect(&r1, &r2)) {
      continue;
    }

    boss.set_life(boss.get_life() - 1);
    if (boss.get_life() <= 0) {
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::boss_down), 0);
    } else {
      Mix_PlayChannel(-1, mixer_manager.get_se(se_type::enemy_hit), 0);
    }
    bullet.make_invisible();
    for (auto &effect : effect.effects) {
      if (effect.view) {
        continue;
      }
      effect.view = true;
      effect.pos.x = -80 + bullet_pos.x + r1.w / 2;
      effect.pos.y = -80 + bullet_pos.y + r1.h / 2;
      effect.count = 0;
      break;
    }
    if (boss.get_life() <= 0) {
      return true;
    }
    break;
  }
  return false;
}

}  // namespace util
