#ifndef MIXER_MANAGER_H
#define MIXER_MANAGER_H

#include <SDL/SDL_mixer.h>
#include <iostream>
#include <memory>

namespace se_type {

enum {
  fighter_shoot = 0,
  enemy_shoot,
  fighter_hit,
  enemy_hit,
  fighter_down,
  boss_down,
  count,
};

}  // namespace se_type

class MixerManager {
  Mix_Music *music_;
  Mix_Chunk *se_list_[se_type::count];

  inline void load_music(const char *path) {
    music_ = Mix_LoadMUS(path);
    if (!music_) {
      throw Mix_GetError();
    }
  }

  inline void load_se(const char *path, const unsigned char se_type) {
    se_list_[se_type] = Mix_LoadWAV(path);
    if (!se_list_[se_type]) {
      throw Mix_GetError();
    }
  }

 public:
  MixerManager() noexcept {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) !=
        0) {
      std::cerr << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    try {
      load_music("./data/tw012.mp3");
      load_se("./data/tm2_wood001.wav", se_type::fighter_shoot);
      load_se("./data/tm2_shoot003.wav", se_type::enemy_shoot);
      load_se("./data/tm2_hit003.wav", se_type::fighter_hit);
      load_se("./data/tm2_fire000.wav", se_type::enemy_hit);
      load_se("./data/glass-break1.wav", se_type::fighter_down);
      load_se("./data/bowling-pin1.wav", se_type::boss_down);
    } catch (const char &e) {
      std::cerr << "error: " << e << '\n';
      exit(EXIT_FAILURE);
    }
  }

  inline Mix_Music *get_music() const noexcept { return music_; }

  inline Mix_Chunk *get_se(const unsigned char se_type) const noexcept {
    return se_list_[se_type];
  }

  ~MixerManager() noexcept {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music_);
    for (const auto &se : se_list_) {
      Mix_FreeChunk(se);
    }
    Mix_CloseAudio();
    atexit(Mix_Quit);
  }
};

#endif
