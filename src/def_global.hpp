#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL_kanji.hpp"
#include "bg.hpp"
#include "boss.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "input.hpp"
#include "vector.hpp"
#include "wipe.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int MAX_FPS = 60;
const int NUM_SNOW = 256;
const int FIGHTER_SHOT_MAX = 32;
const int ENEMY_MAX = 16;
const int ENEMY_SHOT_MAX = 128;
const int EFFECT_MAX = 64;
const int OFFSET_Y = 48;
const int BOSS_SHOT_MAX = 256;

enum {
  MAIN_SHOT_SE = 0,
  ENEMY_SHOT_SE,
  NUM_SE,
  GAME_STATE_TITLE = 0,
  GAME_STATE_START,
  GAME_STATE_CLEAR,
  GAME_STATE_GAME,
  GAME_STATE_OVER,
  GAME_STATE_PAUSE,
  NUM_GAME_STATE,
  ENEMY_1 = 0,
  BOSS_1,
  NUM_ENEMY_TYPE,
  PRESS_KEY_UP = 0,
  PRESS_KEY_DOWN,
  PRESS_KEY_LEFT,
  PRESS_KEY_RIGHT,
  PRESS_KEY_BUTTON_0,
  PRESS_KEY_BUTTON_1,
  PRESS_KEY_SPACE,
  NUM_PRESS_KEY
};

enum BOSS_STATE {
  BOSS_STATE_AUTOMOVE = 0,
  BOSS_STATE_ATTACK_00,
  BOSS_STATE_ATTACK_01,
  BOSS_STATE_ATTACK_02,
  NUM_BOSS_STATE
};

typedef struct _Shot_data {
  bool view;
  Vector pos;
  Vector move;
} Shot_data;

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
GLOBAL Mix_Music *Music;
GLOBAL Mix_Chunk *Se[2];
GLOBAL Fighter_data Fighter;
GLOBAL Enemy_data Enemy[ENEMY_MAX];
GLOBAL Shot_data Fighter_shot[FIGHTER_SHOT_MAX];
GLOBAL Shot_data Enemy_shot[ENEMY_SHOT_MAX];
GLOBAL Effect_data Effect[EFFECT_MAX];
GLOBAL Boss_data Boss;
GLOBAL Boss_shot_data Boss_shot[BOSS_SHOT_MAX];
GLOBAL bool Press_key[NUM_PRESS_KEY];
GLOBAL bool Edge_key[NUM_PRESS_KEY];

#endif
