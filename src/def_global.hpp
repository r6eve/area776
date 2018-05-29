#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <math.h>
#include "point.hpp"

namespace screen {

enum {
  width = 640,
  height = 480,
  bpp = 32,
  max_fps = 60,
};

}  // namespace screen

enum class enemy_type {
  enemy,
  boss,
};

const int FIGHTER_SHOT_MAX = 32;
const int ENEMY_MAX = 16;
const int ENEMY_SHOT_MAX = 128;
const int EFFECT_MAX = 64;
const int BOSS_SHOT_MAX = 256;

enum class boss_state {
  automove,
  attack00,
  attack01,
  attack02,
};

struct FighterData {
  struct Bullet {
    bool view;
    Point pos;
    Point move;
  };

  Point pos;
  int shot_timer;
  int life;
  Bullet bullets[FIGHTER_SHOT_MAX];
};

struct EnemiesData {
  struct EnemyData {
    bool view;
    Point pos;
    Point move;
    int shot_timer;
  };

  struct Bullet {
    bool view;
    Point pos;
    Point move;
  };

  int life;
  EnemyData enemies[ENEMY_MAX];
  Bullet bullets[ENEMY_SHOT_MAX];
};

struct EffectData {
  bool view;
  Point pos;
  int count;
};

struct BossData {
  struct Bullet {
    bool view;
    int rot;
    int count;
    Point pos;
    Point move;
  };

  boss_state state;
  int x;
  int y;
  int move;
  int shot_rot;
  int shot_count;
  int life;
  Bullet bullets[BOSS_SHOT_MAX];
};

#ifdef MAIN
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL enemy_type Enemy_select;
GLOBAL FighterData Fighter;
GLOBAL EnemiesData Enemy;
GLOBAL EffectData Effect[EFFECT_MAX];
GLOBAL BossData Boss;

#endif
