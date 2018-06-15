#ifndef AREA776_H
#define AREA776_H

#include <iomanip>
#include <memory>
#include <sstream>
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

  const bool fullscreen_mode_;
  const bool debug_mode_;

  SDL_Window *window_;
  SDL_Renderer *renderer_;
  int blink_count_;
  int game_count_;
  game_state game_state_;
  int game_level_;
  enemy_type enemy_select_;

  std::unique_ptr<Fighter> fighter_;
  std::unique_ptr<Enemies> enemies_;
  std::unique_ptr<Boss> boss_;
  std::unique_ptr<Effects> effects_;
  std::unique_ptr<Wipe> wipe_;
  std::unique_ptr<Snow> snow_;
  FontManager font_manager_;
  ImageManager image_manager_;
  InputManager input_manager_;
  MixerManager mixer_manager_;

  void game_title() noexcept;
  void game_start() noexcept;
  void play_game() noexcept;
  void game_clear() noexcept;
  void game_over() noexcept;
  void game_pause() noexcept;

  inline void draw_text(const unsigned char font_size, const RGB &rgb,
                        const Point &p, const char *str) const noexcept {
    const SDL_Color color = {rgb.r, rgb.g, rgb.b, 255};
    SDL_Surface *font_surface =
        TTF_RenderUTF8_Blended(font_manager_.get(font_size), str, color);
    SDL_Texture *font_texture =
        SDL_CreateTextureFromSurface(renderer_, font_surface);
    SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                    static_cast<Uint16>(font_surface->h)};
    SDL_Rect dst;
    dst.x = static_cast<Sint16>(p.x);
    dst.y = static_cast<Sint16>(p.y);
    SDL_QueryTexture(font_texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer_, font_texture, &src, &dst);
    SDL_DestroyTexture(font_texture);
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

  inline void draw_life() noexcept {
    switch (enemy_select_) {
      case enemy_type::enemy: {
        std::stringstream ss;
        ss << "ENEMY LIFE:  " << enemies_->get_life();
        draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
        break;
      }
      case enemy_type::boss: {
        std::stringstream ss;
        ss << "BOSS LIFE:  " << boss_->get_life();
        draw_text(font_size::x16, rgb::white, Point{32, 24}, ss.str().c_str());
        break;
      }
    }

    std::stringstream ss;
    ss << "LIFE:  " << fighter_->get_life();
    draw_text(font_size::x16, rgb::white, fighter_->get_pos() + Point{0, 55},
              ss.str().c_str());
  }

  inline bool poll_event() noexcept {
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
        default:
          // do nothing
          break;
      }
    }
    return true;
  }

  inline void wait_game() noexcept {
    static Uint32 pre_count;
    const double wait_time = 1000.0 / screen::max_fps;
    const Uint32 wait_count = (wait_time + 0.5);
    if (pre_count) {
      const Uint32 now_count = SDL_GetTicks();
      const Uint32 interval = now_count - pre_count;
      if (interval < wait_count) {
        const Uint32 delay_time = wait_count - interval;
        SDL_Delay(delay_time);
      }
    }
    pre_count = SDL_GetTicks();
  }

  inline void draw_fps() noexcept {
    static Uint32 pre_count;
    const Uint32 now_count = SDL_GetTicks();
    if (pre_count) {
      static double frame_rate;
      Uint32 mut_interval = now_count - pre_count;
      if (mut_interval < 1) {
        mut_interval = 1;
      }
      const Uint32 interval = mut_interval;

      if (!(pre_count % 30)) {
        frame_rate = 1000.0 / interval;
      }

      std::stringstream ss;
      ss << "FrameRate[" << std::setprecision(2)
         << std::setiosflags(std::ios::fixed) << frame_rate << "]";
      draw_text(font_size::x16, rgb::green, Point{screen::width - 140, 16},
                ss.str().c_str());
    }
    pre_count = now_count;
  }

  inline void draw_map() noexcept {
    SDL_Texture *map_texture = image_manager_.get(renderer_, image::map);
    SDL_Rect src = {0, 0, screen::width, screen::height};
    SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_RenderCopy(renderer_, map_texture, &src, &dst);
    SDL_DestroyTexture(map_texture);
  }

  inline void draw_translucence() noexcept {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface *trans_surface =
        SDL_CreateRGBSurface(SDL_SWSURFACE, screen::width, screen::height, 32,
                             rmask, gmask, bmask, amask);
    if (trans_surface == nullptr) {
      std::cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    SDL_Texture *trans_texture =
        SDL_CreateTextureFromSurface(renderer_, trans_surface);
    SDL_FreeSurface(trans_surface);
    SDL_Rect src = {0, 0, screen::width, screen::height};
    SDL_Rect dst = {0, 0, screen::width, screen::height};
    SDL_RenderCopy(renderer_, trans_texture, &src, &dst);
    SDL_DestroyTexture(trans_texture);
    if (blink_count_ < 30) {
      draw_text(font_size::x36, rgb::white, Point{220, 180}, "P a u s e");
      ++blink_count_;
    } else if (blink_count_ < 60) {
      ++blink_count_;
    } else {
      blink_count_ = 0;
    }
  }

 public:
  Area776(const bool fullscreen_mode, const bool debug_mode) noexcept
      : fullscreen_mode_(fullscreen_mode),
        debug_mode_(debug_mode),
        blink_count_(0),
        game_count_(0),
        game_state_(game_state::title) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen_mode_) {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    window_ = SDL_CreateWindow("Area776", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, screen::width,
                               screen::height, flags);
    if (window_ == nullptr) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    fighter_ = std::make_unique<Fighter>(Fighter(renderer_));
    enemies_ = std::make_unique<Enemies>(Enemies(renderer_));
    boss_ = std::make_unique<Boss>(Boss(renderer_));
    effects_ = std::make_unique<Effects>(Effects(renderer_));
    wipe_ = std::make_unique<Wipe>(Wipe(renderer_));
    snow_ = std::make_unique<Snow>(Snow(renderer_));
    SDL_ShowCursor(SDL_DISABLE);
  }

  void run() noexcept;

  ~Area776() noexcept { atexit(SDL_Quit); }
};

#endif
