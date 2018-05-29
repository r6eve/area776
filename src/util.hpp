#ifndef UTIL_H
#define UTIL_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"

namespace util {

/**
 * Return true if player life is 0.
 */
bool check_enemyshots_hit_fighter(enemy_type enemy_select, Fighter &fighter,
                                  Enemy &enemy, Boss &boss,
                                  Effect &effect) noexcept;

/**
 * Return true if enemy life is 0.
 */
bool check_fightershots_hit_enemy(Fighter &fighter, Enemy &enemy,
                                  Effect &effect) noexcept;

/**
 * Return true if boss life is 0.
 */
bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss,
                                 Effect &effect) noexcept;

}  // namespace util

#endif
