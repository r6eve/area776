#ifndef VECTOR_H
#define VECTOR_H

#include "def_global.hpp"

struct Vector {
  double x;
  double y;

  inline void add(Vector &src1, Vector &src2) {
    x = src1.x + src2.x;
    y = src1.y + src2.y;
  }

  inline void add(Vector &src) {
    x += src.x;
    y += src.y;
  }

  inline void sub(Vector &src1, Vector &src2) {
    x = src1.x - src2.x;
    y = src1.y - src2.y;
  }

  inline void sub(Vector &src) {
    x -= src.x;
    y -= src.y;
  }

  inline void mul(Vector &src, double d) {
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

  inline void copy(Vector &src) {
    x = src.x;
    y = src.y;
  }

  inline void rot(Vector &src, double r) {
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
