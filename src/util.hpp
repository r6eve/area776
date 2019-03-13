//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

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
                                  Enemies &enemies, Boss &boss,
                                  Effects &effects,
                                  const MixerManager &mixer_manager) noexcept;

/**
 * Return true if enemies life is 0.
 */
bool check_fightershots_hit_enemy(Fighter &fighter, Enemies &enemies,
                                  Effects &effects,
                                  const MixerManager &mixer_manager) noexcept;

/**
 * Return true if boss life is 0.
 */
bool check_fightershots_hit_boss(Fighter &fighter, Boss &boss, Effects &effects,
                                 const MixerManager &mixer_manager) noexcept;

}  // namespace util
