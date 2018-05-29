#ifndef UTIL_H
#define UTIL_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"

namespace util {

bool check_hit_rect(SDL_Rect *a, SDL_Rect *b);

/**
 * Return true if player life is 0.
 */
bool check_enemyshots_hit_fighter(enemy_type enemy_select, Fighter &fighter,
                                  Enemy &enemy, Boss &boss, Effect &effect);

/**
 * Return true if enemy life is 0.
 */
bool check_fightershots_hit_enemy(Fighter &fighter, Enemy &enemy,
                                  Effect &effect);

/**
 * Return true if boss life is 0.
 */
bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss, Effect &effect);

}  // namespace util

#endif
