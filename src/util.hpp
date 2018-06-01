#ifndef UTIL_H
#define UTIL_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "mixer_manager.hpp"

namespace util {

/**
 * Return true if player life is 0.
 */
bool check_enemyshots_hit_fighter(enemy_type enemy_select, Fighter &fighter,
                                  Enemies &enemies, Boss &boss, Effect &effect,
                                  const MixerManager &mixer_manager) noexcept;

/**
 * Return true if enemies life is 0.
 */
bool check_fightershots_hit_enemy(Fighter &fighter, Enemies &enemies,
                                  Effect &effect,
                                  const MixerManager &mixer_manager) noexcept;

/**
 * Return true if boss life is 0.
 */
bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss, Effect &effect,
                                 const MixerManager &mixer_manager) noexcept;

}  // namespace util

#endif
