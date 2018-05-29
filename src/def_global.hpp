#ifndef DEF_GLOBAL_H
#define DEF_GLOBAL_H

#include <SDL/SDL.h>
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

  inline bool operator!=(const Point &rhs) const noexcept {
    return ((x != rhs.x) || (y != rhs.y));
  }

  inline Point operator+(const Point &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y};
  }

  inline void operator+=(const Point &rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
  }

  inline void add(Point &src1, Point &src2) {
    x = src1.x + src2.x;
    y = src1.y + src2.y;
  }

  inline void add(Point &src) {
    x += src.x;
    y += src.y;
  }

  inline void sub(Point &src1, Point &src2) {
    x = src1.x - src2.x;
    y = src1.y - src2.y;
  }

  inline void sub(Point &src) {
    x -= src.x;
    y -= src.y;
  }

  inline void mul(Point &src, double d) {
    x = src.x * d;
    y = src.y * d;
  }

  inline void mul(double d) {
    x *= d;
    y *= d;
  }

  inline void norm() {
    const double d = 1 / hypot(x, y);
    x *= d;
    y *= d;
  }

  inline void copy(Point &src) {
    x = src.x;
    y = src.y;
  }

  inline void rot(Point &src, double r) {
    x = src.x * cos(r) + src.y * -sin(r);
    y = src.x * sin(r) + src.y * cos(r);
  }

  inline void rot(double r) {
    const double pre_x = x;
    const double pre_y = y;
    x = pre_x * cos(r) + pre_y * -sin(r);
    y = pre_x * sin(r) + pre_y * cos(r);
  }
};

#endif
