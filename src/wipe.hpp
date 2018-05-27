#ifndef WIPE_H
#define WIPE_H

#include "def_global.hpp"

class Wipe {
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;

  enum class wipe_type { in, out };

  wipe_type wipe_variation_;
  unsigned int wipe_count_;
  unsigned int wipe_count_reach_;

 public:
  Wipe() {}

  /**
   * Set wipe-in mode for drawing.
   */
  inline void set_wipe_in() noexcept {
    wipe_variation_ = wipe_type::in;
    wipe_count_ = SCREEN_HEIGHT / 10;
    wipe_count_reach_ = 0;
  }

  /**
   * Set wipe-out mode for drawing.
   */
  inline void set_wipe_out() noexcept {
    wipe_variation_ = wipe_type::out;
    wipe_count_ = 0;
    wipe_count_reach_ = SCREEN_HEIGHT / 10;
  }

  /**
   * Start drawing. Before execute this function, call set_wipe_in() or
   * set_wipe_out().
   */
  inline void draw(SDL_Surface *screen) const noexcept {
    SDL_Rect dst = {0, 0, SCREEN_WIDTH, static_cast<Uint16>(wipe_count_)};
    const unsigned int dy = SCREEN_HEIGHT / 10;
    Uint32 black = 0x00000000;
    for (dst.y = 0; dst.y < SCREEN_HEIGHT; dst.y += dy) {
      SDL_FillRect(screen, &dst, black);
    }
  }

  /**
   * Return true if wipe finishes.
   */
  inline bool update() noexcept {
    if (wipe_count_ == wipe_count_reach_) {
      return true;
    }

    switch (wipe_variation_) {
      case wipe_type::in:
        --wipe_count_;
        break;
      case wipe_type::out:
        ++wipe_count_;
        break;
      default:
        // NOTREACHED
        break;
    }
    return false;
  }

  ~Wipe() {}
};

#endif
