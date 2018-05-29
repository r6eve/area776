#include "util.hpp"
#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"

namespace util {

inline bool check_hit_rect(SDL_Rect *a, SDL_Rect *b) noexcept {
  return ((a->x) < (b->x + b->w)) && ((b->x) < (a->x + a->w)) &&
         ((a->y) < (b->y + b->h)) && ((b->y) < (a->y + a->h));
}

bool check_enemyshots_hit_fighter(enemy_type enemy_select, Fighter &fighter,
                                  Enemy &enemy, Boss &boss,
                                  Effect &effect) noexcept {
  SDL_Rect r1 = {static_cast<Sint16>(fighter.pos.x + 20),
                 static_cast<Sint16>(fighter.pos.y + 16), 20, 22};

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

        --fighter.life;
        bullet.view = false;
        for (auto &effect : effect.effects) {
          if (effect.view) {
            continue;
          }

          effect.view = true;
          effect.pos.x = -80 + fighter.pos.x + 30;
          effect.pos.y = -80 + fighter.pos.y + 30;
          effect.count = 0;
          break;
        }
      }
      break;
    }
    case enemy_type::boss: {
      for (auto &bullet : boss.bullets) {
        if (!bullet.view) {
          continue;
        }

        SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x + 3),
                       static_cast<Sint16>(bullet.pos.y + 3), 10, 10};
        if (!check_hit_rect(&r1, &r2)) {
          continue;
        }

        --fighter.life;
        bullet.view = false;
        for (auto &effect : effect.effects) {
          if (effect.view) {
            continue;
          }

          effect.view = true;
          effect.pos.x = -80 + fighter.pos.x + 30;
          effect.pos.y = -80 + fighter.pos.y + 30;
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

  return fighter.life <= 0;
}

bool check_fightershots_hit_enemy(Fighter &fighter, Enemy &enemy,
                                  Effect &effect) noexcept {
  for (auto &e : enemy.enemies) {
    if (!e.view) {
      continue;
    }

    for (auto &bullet : fighter.bullets) {
      if (!bullet.view) {
        continue;
      }

      SDL_Rect r1 = {static_cast<Sint16>(e.pos.x), static_cast<Sint16>(e.pos.y),
                     35, 35};
      SDL_Rect r2 = {static_cast<Sint16>(bullet.pos.x),
                     static_cast<Sint16>(bullet.pos.y), 10, 24};
      if (!check_hit_rect(&r1, &r2)) {
        continue;
      }

      ++enemy.life;
      e.view = false;
      bullet.view = false;
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
      if (enemy.life > 29) {
        return true;
      }

      break;
    }
  }
  return false;
}

bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss,
                                 Effect &effect) noexcept {
  for (auto &bullet : fighter.bullets) {
    if (!bullet.view) {
      continue;
    }
    SDL_Rect r1 = {static_cast<Sint16>(bullet.pos.x),
                   static_cast<Sint16>(bullet.pos.y), 10, 24};
    SDL_Rect r2 = {static_cast<Sint16>(boss.x + 171),
                   static_cast<Sint16>(boss.y + 95), 57, 57};
    if (!check_hit_rect(&r1, &r2)) {
      continue;
    }
    ++boss.life;
    bullet.view = false;
    for (auto &effect : effect.effects) {
      if (effect.view) {
        continue;
      }
      effect.view = true;
      effect.pos.x = -80 + bullet.pos.x + r1.w / 2;
      effect.pos.y = -80 + bullet.pos.y + r1.h / 2;
      effect.count = 0;
      break;
    }
    if (boss.life > 99) {
      return true;
    }
    break;
  }
  return false;
}

}  // namespace util
