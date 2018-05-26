#include "def_global.hpp"

void add_vec(Vector *dst, Vector *src1, Vector *src2) {
  dst->x = src1->x + src2->x;
  dst->y = src1->y + src2->y;
}
void add_vec(Vector *dst, Vector *src) {
  dst->x += src->x;
  dst->y += src->y;
}

void sub_vec(Vector *dst, Vector *src1, Vector *src2) {
  dst->x = src1->x - src2->x;
  dst->y = src1->y - src2->y;
}
void sub_vec(Vector *dst, Vector *src) {
  dst->x -= src->x;
  dst->y -= src->y;
}

void mul_vec(Vector *dst, Vector *src, double d) {
  dst->x = src->x * d;
  dst->y = src->y * d;
}
void mul_vec(Vector *dst, double d) {
  dst->x *= d;
  dst->y *= d;
}

double get_vec_length(Vector *dst) {
  return hypot(dst->x, dst->y);
}

void normalize_vec(Vector *dst) {
  double l = get_vec_length(dst);
  mul_vec(dst, 1 / l);
}

void cp_vec(Vector *dst, Vector *src) { memcpy(dst, src, sizeof(Vector)); }

void rot_vec(Vector *dst, Vector *src, double r) {
  dst->x = src->x * cos(r) + src->y * -sin(r);
  dst->y = src->x * sin(r) + src->y * cos(r);
}
void rot_vec(Vector *dst, double r) {
  Vector vec;
  cp_vec(&vec, dst);
  rot_vec(dst, &vec, r);
}
