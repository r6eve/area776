#ifndef AREA776_H
#define AREA776_H

#include "def_global.hpp"
#include "font_manager.hpp"
#include "image_manager.hpp"
#include "input_manager.hpp"
#include "mixer_manager.hpp"
#include "wipe.hpp"

struct Point {
  int x;
  int y;

  bool operator==(const Point &rhs) const noexcept {
    return ((x == rhs.x) && (y == rhs.y));
  }

  bool operator!=(const Point &rhs) const noexcept {
    return ((x != rhs.x) || (y != rhs.y));
  }

  Point operator+(const Point &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  void operator+=(const Point &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

 public:
  unsigned int distance(const Point &rhs) const noexcept {
    return static_cast<unsigned int>(
        hypot(static_cast<double>(x - rhs.x), static_cast<double>(y - rhs.y)));
  }
};

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
  game_state game_state_;

  Wipe wipe_;
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
      : game_state_(game_state::title), debug_mode_(debug_mode) {}

  bool init();
  bool init_sdl();
  void main_loop();
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
