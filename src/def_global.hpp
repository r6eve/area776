#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <math.h>
#include "boss.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "vector.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int MAX_FPS = 60;
const int NUM_SNOW = 256;
const int FIGHTER_SHOT_MAX = 32;
const int ENEMY_MAX = 16;
const int ENEMY_SHOT_MAX = 128;
const int EFFECT_MAX = 64;
const int BOSS_SHOT_MAX = 256;

enum {
  GAME_STATE_TITLE = 0,
  GAME_STATE_START,
  GAME_STATE_CLEAR,
  GAME_STATE_GAME,
  GAME_STATE_OVER,
  GAME_STATE_PAUSE,
  NUM_GAME_STATE,
};

enum {
  ENEMY_1 = 0,
  BOSS_1,
  NUM_ENEMY_TYPE,
};

enum BOSS_STATE {
  BOSS_STATE_AUTOMOVE = 0,
  BOSS_STATE_ATTACK_00,
  BOSS_STATE_ATTACK_01,
  BOSS_STATE_ATTACK_02,
  NUM_BOSS_STATE
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

GLOBAL SDL_Surface *Screen;
GLOBAL int Enemy_life;
GLOBAL int Boss_life;
GLOBAL int Chara_life;
GLOBAL int Game_count;
GLOBAL int Game_level;
GLOBAL int Game_state;
GLOBAL int Enemy_select;
GLOBAL Fighter_data Fighter;
GLOBAL Enemy_data Enemy[ENEMY_MAX];
GLOBAL Shot_data Fighter_shot[FIGHTER_SHOT_MAX];
GLOBAL Shot_data Enemy_shot[ENEMY_SHOT_MAX];
GLOBAL Effect_data Effect[EFFECT_MAX];
GLOBAL Boss_data Boss;
GLOBAL Boss_shot_data Boss_shot[BOSS_SHOT_MAX];

#endif
