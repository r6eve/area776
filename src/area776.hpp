#ifndef AREA776_H
#define AREA776_H

#include "boss.hpp"
#include "def_global.hpp"
#include "effect.hpp"
#include "enemy.hpp"
#include "fighter.hpp"
#include "font_manager.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "snow.hpp"
#include "wipe.hpp"

struct RGB {
  Uint8 r;
  Uint8 g;
  Uint8 b;
};

namespace rgb {

const RGB black = RGB{0x00, 0x00, 0x00};
const RGB red = RGB{0xff, 0x00, 0x00};
const RGB dark_red = RGB{0xb0, 0x00, 0x00};
const RGB green = RGB{0x00, 0xff, 0x00};
const RGB white = RGB{0xff, 0xff, 0xff};

}  // namespace rgb

class Area776 {
  enum class game_state {
    title,
    start,
    clear,
    playing,
    gameover,
    pause,
  };

  const bool debug_mode_;

  SDL_Surface *screen_;
  int blink_count_;
  int game_count_;
  game_state game_state_;
  int game_level_;
  enemy_type enemy_select_;

  Fighter fighter_;
  Enemy enemy_;
  Boss boss_;
  Effect effect_;

  Wipe wipe_;
  Snow snow_;
  FontManager font_manager_;
  ImageManager image_manager_;
  InputManager input_manager_;
  MixerManager mixer_manager_;

  inline void draw_text(const unsigned char font_size, const RGB &rgb,
                        const Point &p, const char *str) const noexcept {
    const SDL_Color color = {rgb.r, rgb.g, rgb.b, 255};
    SDL_Surface *font_surface =
        TTF_RenderUTF8_Blended(font_manager_.get(font_size), str, color);
    SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                    static_cast<Uint16>(font_surface->h)};
    SDL_Rect dst = {static_cast<Sint16>(p.x), static_cast<Sint16>(p.y), 0, 0};
    SDL_BlitSurface(font_surface, &src, screen_, &dst);
  }

  inline void draw_text(const unsigned char font_size, const RGB &&rgb,
                        const Point &p, const char *str) const noexcept {
    draw_text(font_size, rgb, p, str);
  }

  inline void draw_text(const unsigned char font_size, const RGB &rgb,
                        const Point &&p, const char *str) const noexcept {
    draw_text(font_size, rgb, p, str);
  }

  inline void draw_text(const unsigned char font_size, const RGB &&rgb,
                        const Point &&p, const char *str) const noexcept {
    draw_text(font_size, rgb, p, str);
  }

 public:
  Area776(const bool debug_mode) noexcept
      : debug_mode_(debug_mode),
        blink_count_(0),
        game_count_(0),
        game_state_(game_state::title) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("SDL_SHOOTING", nullptr);
    if (debug_mode_) {
      screen_ = SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                                 SDL_HWSURFACE | SDL_DOUBLEBUF);
    } else {
      screen_ =
          SDL_SetVideoMode(screen::width, screen::height, screen::bpp,
                           SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    }
    if (!screen_) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    SDL_ShowCursor(SDL_DISABLE);
  }

  void run();
  void game_title();
  void game_start();
  void play_game();
  void game_clear();
  void game_over();
  void game_pause();
  void draw_life();
  bool poll_event();
  void wait_game();
  void draw_fps();
  void draw_map();
  void draw_translucence();
  ~Area776() noexcept { atexit(SDL_Quit); }
};

#endif
