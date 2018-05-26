#ifndef VECTOR_H
#define VECTOR_H

struct Vector {
    double x;
    double y;
};

void add_vec(Vector *dst, Vector *src1, Vector *src2);
void add_vec(Vector *dst, Vector *src);
void sub_vec(Vector *dst, Vector *src1, Vector *src2);
void sub_vec(Vector *dst, Vector *src);
void mul_vec(Vector *dst, Vector *src, double d);
void mul_vec(Vector *dst, double d);
double get_vec_length(Vector *dst);
void normalize_vec(Vector *dst);
void cp_vec(Vector *dst, Vector *src);
void rot_vec(Vector *dst, Vector *src, double r);
void rot_vec(Vector *dst, double r);

#endif
