//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL2/SDL.h>
#include <cmath>

const double PI = 4 * atan(1);

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

struct Point {
  double x;
  double y;

  inline bool operator==(const Point &rhs) const noexcept {
    return ((x == rhs.x) && (y == rhs.y));
  }

  inline bool operator==(const Point &&rhs) const noexcept {
    return ((x == rhs.x) && (y == rhs.y));
  }

  inline bool operator!=(const Point &rhs) const noexcept {
    return ((x != rhs.x) || (y != rhs.y));
  }

  inline bool operator!=(const Point &&rhs) const noexcept {
    return ((x != rhs.x) || (y != rhs.y));
  }

  inline Point operator+(const double d) const noexcept {
    return Point{x + d, y + d};
  }

  inline Point operator+(const Point &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  inline Point operator+(const Point &&rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  inline void operator+=(const double d) noexcept {
    x += d;
    y += d;
  }

  inline void operator+=(const Point &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

  inline void operator+=(const Point &&rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

  inline Point operator-(const double d) const noexcept {
    return Point{x - d, y - d};
  }

  inline Point operator-(const Point &rhs) const noexcept {
    return Point{x - rhs.x, y - rhs.y};
  }

  inline Point operator-(const Point &&rhs) const noexcept {
    return Point{x - rhs.x, y - rhs.y};
  }

  inline void operator-=(const double d) noexcept {
    x -= d;
    y -= d;
  }

  inline void operator-=(const Point &rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
  }

  inline void operator-=(const Point &&rhs) noexcept {
    x -= rhs.x;
    y -= rhs.y;
  }

  inline Point operator*(const double d) const noexcept {
    return Point{x * d, y * d};
  }

  inline void operator*=(const double d) noexcept {
    x *= d;
    y *= d;
  }

  inline void norm() noexcept {
    const double d = 1 / hypot(x, y);
    x *= d;
    y *= d;
  }

  inline void rot(const Point &src, double r) noexcept {
    x = src.x * cos(r) + src.y * -sin(r);
    y = src.x * sin(r) + src.y * cos(r);
  }

  inline void rot(const Point &&src, double r) noexcept {
    x = src.x * cos(r) + src.y * -sin(r);
    y = src.x * sin(r) + src.y * cos(r);
  }

  inline void rot(double r) noexcept {
    const double pre_x = x;
    const double pre_y = y;
    x = pre_x * cos(r) + pre_y * -sin(r);
    y = pre_x * sin(r) + pre_y * cos(r);
  }
};

#endif
