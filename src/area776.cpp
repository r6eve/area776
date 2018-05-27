#define MAIN

#include "area776.hpp"

static Kanji_Font *Font[2];
enum { FONT_SIZE_16, FONT_SIZE_24, NUM_FONT };
static int Blink_count;

bool init() {
  if (!init_sdl()) {
    return false;
  } else if (!init_audio()) {
    end();
    return false;
  } else if (!init_font()) {
    end();
    return false;
  } else if (!init_game()) {
    end();
    return false;
  } else {
    init_color();
    init_joystick();
    return true;
  }
}

bool init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "Can't initialize sdl. : %s\n", SDL_GetError());
    return false;
  }
  SDL_WM_SetCaption("SDL_SHOOTING", NULL);
  // Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
  // SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  Screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                            SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (!Screen) {
    fprintf(stderr, "Can't initialize screen. : %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }
  SDL_ShowCursor(SDL_DISABLE);

  return true;
}

bool init_audio() {
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    fprintf(stderr, "Can't initialize sdl_mixer. : %s\n", Mix_GetError());
    return false;
  }
  if ((Music = Mix_LoadMUS("./data/batof2s.mp3")) == NULL) {
    fprintf(stderr, "Can't load se. : %s\n", Mix_GetError());
    return false;
  }
  if ((Se[MAIN_SHOT_SE] = Mix_LoadWAV("./data/tm2_wood001.wav")) == NULL) {
    fprintf(stderr, "Can't load se. : %s\n", Mix_GetError());
    return false;
  }
  if ((Se[ENEMY_SHOT_SE] = Mix_LoadWAV("./data/tm2_shoot003.wav")) == NULL) {
    fprintf(stderr, "Can't load se. : %s\n", Mix_GetError());
    return false;
  }

  return true;
}

bool init_font() {
  Font[FONT_SIZE_16] = Kanji_OpenFont("./data/jiskan16.bdf", 16);
  Kanji_AddFont(Font[FONT_SIZE_16], "./data/8x16.bdf");
  Font[FONT_SIZE_24] = Kanji_OpenFont("./data/jiskan24.bdf", 24);
  Kanji_AddFont(Font[FONT_SIZE_24], "./data/12x24.bdf");
  for (int i = 0; i < NUM_FONT; ++i) {
    if (Font[i] == NULL) {
      fprintf(stderr, "Can't initialize Font[%d]. : %s\n", i, SDL_GetError());
      return false;
    } else {
      Kanji_SetCodingSystem(Font[i], KANJI_UTF8);
    }
  }

  return true;
}

bool init_game() {
  if (!load_img("./data/fighter.png", "fighter")) {
    return false;
  }
  if (!load_img("./data/mons13.png", "mons13")) {
    return false;
  }
  if (!load_img("./data/boss.png", "boss")) {
    return false;
  }
  if (!load_img("./data/oval_re.png", "oval_re")) {
    return false;
  }
  if (!load_img("./data/bm01.png", "bm01")) {
    return false;
  }
  if (!load_img("./data/effect01.png", "effect01")) {
    return false;
  }
  if (!load_img("./data/snow.png", "snow")) {
    return false;
  }
  if (!load_img("./data/72.png", "map")) {
    return false;
  }
  Game_count = 0;
  Game_state = GAME_STATE_TITLE;
  Blink_count = 0;

  return true;
}

void main_loop() {
  for (;;) {
    update_input();
    switch (Game_state) {
      case GAME_STATE_TITLE:
        title();
        break;
      case GAME_STATE_START:
        game_start();
        break;
      case GAME_STATE_GAME:
        play_game();
        break;
      case GAME_STATE_CLEAR:
        game_clear();
        break;
      case GAME_STATE_OVER:
        game_over();
        break;
      case GAME_STATE_PAUSE:
        game_pause();
        break;
    }
    if (!poll_event()) {
      return;
    }
#ifdef DEBUG
    draw_fps();
#endif
    SDL_Flip(Screen);
    wait_game();
  }
}

void title() {
  SDL_Rect dst_back = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  Uint32 col = 0xffffffff;
  SDL_FillRect(Screen, &dst_back, col);
  switch (Game_count) {
    case 0:
      set_wipe_in();
      ++Game_count;
      break;
    case 1:
      Kanji_PutText(Screen, 160, 180, Font[FONT_SIZE_24], BLACK,
                    "S H O O T I N G - G A M E");
      draw_wipe(0);
      if (update_wipe()) {
        ++Game_count;
      }
      break;
    case 2:
      Kanji_PutText(Screen, 160, 180, Font[FONT_SIZE_24], BLACK,
                    "S H O O T I N G - G A M E");
      Kanji_PutText(Screen, 240, 300, Font[FONT_SIZE_16], BLACK,
                    "P r e s s  Z");
      ++Blink_count;
      if (Blink_count >= 20) {
        SDL_Rect dst_back = {240, 300, SCREEN_WIDTH - 240, SCREEN_HEIGHT - 300};
        Uint32 col = 0xffffffff;
        SDL_FillRect(Screen, &dst_back, col);
        if (Blink_count >= 40) {
          Blink_count = 0;
        }
      }
      if (Press_key[PRESS_KEY_BUTTON_0]) {
        set_wipe_out();
        ++Game_count;
      }
      break;
    case 3:
      Kanji_PutText(Screen, 160, 180, Font[FONT_SIZE_24], BLACK,
                    "S H O O T I N G - G A M E");
      Kanji_PutText(Screen, 240, 300, Font[FONT_SIZE_16], BLACK,
                    "P r e s s  Z");
      draw_wipe(0);
      Mix_PlayMusic(Music, -1);
      if (update_wipe()) {
        init_fighter();
        init_enemy();
        init_effect();
        init_bg();
        init_boss();
        Game_count = 0;
        Game_state = GAME_STATE_START;
        Game_level = 1;
        Enemy_select = ENEMY_1;
        Chara_life = 20;
        Enemy_life = 0;
        Boss_life = 0;
        srand((unsigned int)time(NULL));
      }
      break;
  }
}

void game_start() {
  mv_fighter();
  mv_fighter_shot();
  draw_map();
  draw_fighter();
  draw_fighter_shot();

  if (Game_count == 0) {
    set_wipe_in();
    draw_wipe(0);
    ++Game_count;
  } else if (Game_count == 1) {
    draw_wipe(0);
    if (update_wipe()) {
      ++Game_count;
    }
  } else {
    ++Game_count;
  }

  if (Game_count < 130) {
    Kanji_PutText(Screen, 272, 232, Font[FONT_SIZE_16], RED, "S t a g e %d",
                  Game_level);
  } else if (Game_count < 200) {
    Kanji_PutText(Screen, 284, 232, Font[FONT_SIZE_16], RED, "S t a r t");
  }

  if (Game_count > 220) {
    Game_count = 0;
    Game_state = GAME_STATE_GAME;
  }
}

void play_game() {
  mv_fighter();
  mv_fighter_shot();
  check_enemyshots_hit_mychara(); /* do GameSelect = GAME_STATE_OVER;  */
  update_effect();
  draw_map();
  if (Enemy_select == ENEMY_1) {
    appear_enemy();
    mv_enemy();
    mv_enemy_shot();
    check_myshots_hit_enemy(); /* do Enemy_select = BOSS_1; */
    update_bg();
    draw_bg();
    draw_enemy();
    draw_enemy_shot();
  } else if (Enemy_select == BOSS_1) {
    if ((Game_count < 130) && (Blink_count < 20)) {
      Kanji_PutText(Screen, 272, 232, Font[FONT_SIZE_24], RED, "B O O S  %d",
                    Game_level);
      ++Game_count;
      ++Blink_count;
      if (Blink_count >= 20) {
        if (Blink_count >= 40) {
          Blink_count = 0;
        }
      }
    } else if ((Game_count < 130) && (Blink_count >= 20)) {
      if (Blink_count >= 40) {
        Blink_count = 0;
      }
      ++Game_count;
      ++Blink_count;
    } else {
      mv_boss();
      mv_boss_shot();

      /* In this function, Game_state = GAME_STATE_CLEAR; Game_count = 0 */
      check_myshots_hit_boss();

      draw_boss();
      draw_boss_shot();
    }
  }
  draw_fighter_shot();
  draw_fighter();
  draw_effect();
  draw_life();
}

void game_clear() {
  mv_fighter();
  mv_fighter_shot();
  draw_map();
  draw_fighter();
  draw_fighter_shot();
  draw_life();

  if (Game_count == 0) {
    set_wipe_out();
    draw_wipe(0);
    ++Game_count;
  } else if (Game_count >= 1) {
    draw_wipe(0);
    if (update_wipe()) {
      if (Game_level == 1) {
        SDL_Rect dst_back = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        Uint32 col = 0xffffffff;
        SDL_FillRect(Screen, &dst_back, col);
        Kanji_PutText(Screen, 200, 160, Font[FONT_SIZE_24], BLACK,
                      "G A M E  C L E A R");
        Kanji_PutText(Screen, 200, 280, Font[FONT_SIZE_16], RED,
                      "C o n g r a t u l a t i o n s");
        ++Game_count;
        if (Game_count > 200) {
          draw_wipe(0);
          if (update_wipe()) {
            Game_count = 0;
            Game_state = GAME_STATE_TITLE;
            Mix_HaltMusic();
            Mix_FreeMusic(Music);
          }
        }
      } else {
        init_fighter();
        init_enemy();
        init_boss();
        init_effect();
        init_bg();
        Game_count = 0;
        Game_state = GAME_STATE_START;
        ++Game_level;
        Enemy_life = 0;
        Boss_life = 0;
      }
    }
  }
}

void game_over() {
  Kanji_PutText(Screen, 222, 150, Font[FONT_SIZE_24], RED, "G a m e O v e r");
  ++Game_count;
  if (Game_count > 200) {
    draw_wipe(0);
    if (update_wipe()) {
      Game_count = 0;
      Game_state = GAME_STATE_TITLE;
      Mix_HaltMusic();
      Mix_FreeMusic(Music);
    }
  }
}

void game_pause() {
  draw_map();
  if (Enemy_select == ENEMY_1) {
    draw_bg();
    draw_enemy();
    draw_enemy_shot();
  } else if (Enemy_select == BOSS_1) {
    draw_boss();
    draw_boss_shot();
  }
  draw_fighter_shot();
  draw_fighter();
  draw_effect();
  draw_life();
  draw_translucence();
  if (Edge_key[PRESS_KEY_SPACE]) {
    Game_state = GAME_STATE_GAME;
  }
}

void draw_life() {
  if (Enemy_select == ENEMY_1) {
    Kanji_PutText(Screen, 32, 24, Font[FONT_SIZE_16], RED, "Enemy_life %d",
                  30 - Enemy_life);
  } else if (Enemy_select == BOSS_1) {
    Kanji_PutText(Screen, 32, 24, Font[FONT_SIZE_16], RED, "Boss_life %d",
                  100 - Boss_life);
  }
  Kanji_PutText(Screen, Fighter.pos.x, Fighter.pos.y + 55, Font[FONT_SIZE_16],
                WHITE, "Life %d", Chara_life);
}

bool poll_event() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return false;
        }
        break;
    }
  }
  return true;
}

void wait_game() {
  static Uint32 pre_count;
  double wait_time = 1000.0 / MAX_FPS;
  Uint32 wait_count = (wait_time + 0.5);
  if (pre_count) {
    Uint32 now_count = SDL_GetTicks();
    Uint32 interval = now_count - pre_count;
    if (interval < wait_count) {
      Uint32 delay_time = wait_count - interval;
      SDL_Delay(delay_time);
    }
  }
  pre_count = SDL_GetTicks();
}

void draw_fps() {
  static Uint32 pre_count;
  Uint32 now_count = SDL_GetTicks();
  if (pre_count) {
    Uint32 interval = now_count - pre_count;
    if (interval < 1) {
      interval = 1;
    }
    double frame_rate = 1000.0 / interval;
    Kanji_PutText(Screen, OFFSET_Y + 20, 16, Font[FONT_SIZE_16], GREEN,
                  "FrameRate[%0.2f]", frame_rate);
  }
  pre_count = now_count;
}

void end() {
  del_all_img();
  for (int i = 0; i < NUM_FONT; ++i) {
    Kanji_CloseFont(Font[i]);
  }
  end_joystick();
  end_audio();
  SDL_Quit();
}

void draw_map() {
  SDL_Surface *pSurface = get_img("map");
  SDL_Rect dst = {0, 0};
  SDL_BlitSurface(pSurface, NULL, Screen, &dst);
}

bool check_hit_rect(SDL_Rect *a, SDL_Rect *b) {
  if (((a->x) < (b->x + b->w)) && ((b->x) < (a->x + a->w)) &&
      ((a->y) < (b->y + b->h)) && ((b->y) < (a->y + a->h))) {
    return true;
  } else {
    return false;
  }
}

void end_audio() {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  for (int i = 0; i < NUM_SE; ++i) {
    Mix_FreeChunk(Se[i]);
  }
  Mix_FreeMusic(Music);
  Mix_CloseAudio();
}

void draw_translucence() {
  Uint32 rmask, gmask, bmask;
  Uint8 alpha = 128;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
#endif
  SDL_Rect dst_back;
  dst_back.x = 0;
  dst_back.y = 0;

  SDL_Surface *trans_surface = SDL_CreateRGBSurface(
      SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, rmask, gmask, bmask, 0);
  if (trans_surface == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_SetAlpha(trans_surface, SDL_SRCALPHA, alpha);
  SDL_BlitSurface(trans_surface, NULL, Screen, &dst_back);
  if (Blink_count < 30) {
    Kanji_PutText(Screen, 240, 200, Font[FONT_SIZE_24], WHITE, "P a u s e");
    ++Blink_count;
  } else if (Blink_count < 60) {
    ++Blink_count;
  } else {
    Blink_count = 0;
  }
}
