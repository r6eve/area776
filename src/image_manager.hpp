#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "def_global.hpp"

typedef struct _Img_list Img_list;
struct _Img_list { // 双方向リスト
  char name[64];
  SDL_Surface *img;
  struct _Img_list *pre;
  struct _Img_list *next;
};

bool load_img(const char *filename, const char *name);
SDL_Surface *get_img(const char *name);
void del_img(const char *name);
void del_all_img();

#endif
