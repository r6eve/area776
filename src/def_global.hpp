#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <math.h>
#include "boss.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "vector.hpp"

namespace screen {

enum {
  width = 640,
  height = 480,
  bpp = 32,
  max_fps = 60,
};

}  // namespace screen

const int FIGHTER_SHOT_MAX = 32;
const int ENEMY_MAX = 16;
const int ENEMY_SHOT_MAX = 128;
const int EFFECT_MAX = 64;
const int BOSS_SHOT_MAX = 256;

enum {
  ENEMY_1 = 0,
  BOSS_1,
  NUM_ENEMY_TYPE,
};

struct Shot_data {
  bool view;
  Vector pos;
  Vector move;
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL int Enemy_life;
GLOBAL int Boss_life;
GLOBAL int Chara_life;
GLOBAL int Enemy_select;
GLOBAL Fighter_data Fighter;
GLOBAL Enemy_data Enemy[ENEMY_MAX];
GLOBAL Shot_data Fighter_shot[FIGHTER_SHOT_MAX];
GLOBAL Shot_data Enemy_shot[ENEMY_SHOT_MAX];
GLOBAL Effect_data Effect[EFFECT_MAX];
GLOBAL Boss_data Boss;
GLOBAL Boss_shot_data Boss_shot[BOSS_SHOT_MAX];

#endif
