#include <SDL/SDL_image.h>
#include <string.h>
#include <new>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include "image_manager.hpp"
using namespace std;

static Img_list *Img_list_top = NULL;

bool load_img(const char *filename, const char *name) {
  SDL_Surface *img = IMG_Load(filename);
  if (!img) {
    fprintf(stderr, "Can't open [%s]: %s.\n", filename, IMG_GetError());
    return false;
  }
  try {
    Img_list *new_img = new Img_list;
    strcpy(new_img->name, name);
    new_img->img = img;
    new_img->pre = NULL;
    new_img->next = Img_list_top;
    if (Img_list_top) {
      Img_list_top->pre = new_img;
    }
    Img_list_top = new_img;
    return true;
  } catch (bad_alloc &) {
    fprintf(stderr, "[%s]: out of memory!!\n", filename);
    SDL_FreeSurface(img);
    return false;
  }
}

SDL_Surface *get_img(const char *name) {
  Img_list *p = Img_list_top;
  while (p) {
    if (!strcmp(p->name, name)) {
      return p->img;
    }
    p = p->next;
  }

  return NULL;
}

void del_img(const char *name) {
  Img_list *p = Img_list_top;
  while (p) {
    if (!strcmp(p->name, name)) {
      if (p->next) {
        p->next->pre = p->pre;
      }
      if (p->pre) {
        p->pre->next = p->next;
      } else {
        Img_list_top = p->next;
      }
      SDL_FreeSurface(p->img);
      delete p;
      break;
    }
    p = p->next;
  }
}

void del_all_img() {
  while (Img_list_top) {
    Img_list *p = Img_list_top->next;
    SDL_FreeSurface(Img_list_top->img);
    delete Img_list_top;
    Img_list_top = p;
    if (Img_list_top) {
      Img_list_top->pre = NULL;
    }
  }
}
