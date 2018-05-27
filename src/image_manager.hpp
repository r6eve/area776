#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <SDL/SDL_image.h>
#include <iostream>
#include <vector>

namespace image {

enum {
  fighter = 0,
  mons13,
  boss,
  oval_re,
  bm01,
  effect01,
  snow,
  map,
  count,
};

}  // namespace image

class ImageManager {
  std::vector<SDL_Surface *> images_;

  // In macOS, using the following type makes `error: static_assert failed "the
  // specified hash does not meet the Hash requirements"`. Therefore, use an
  // array to manage images.
  //
  // `std::unordered_map<std::string, std::unique_ptr<SDL_Surface>> image_map_`
  inline void load(const char *path, const unsigned char image_type) {
    SDL_Surface *image = IMG_Load(path);
    if (!image) {
      throw IMG_GetError();
    }
    images_[image_type] = image;
  }

 public:
  ImageManager() noexcept {
    images_.reserve(image::count);

    const int flag = IMG_INIT_PNG;
    if ((IMG_Init(flag) & flag) != flag) {
      std::cerr << "error: " << IMG_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    try {
      load("./data/fighter.png", image::fighter);
      load("./data/mons13.png", image::mons13);
      load("./data/boss.png", image::boss);
      load("./data/oval_re.png", image::oval_re);
      load("./data/bm01.png", image::bm01);
      load("./data/effect01.png", image::effect01);
      load("./data/snow.png", image::snow);
      load("./data/72.png", image::map);
    } catch (const char &e) {
      std::cerr << "error: " << e << '\n';
      exit(EXIT_FAILURE);
    }
  }

  inline SDL_Surface *get(const unsigned char image_type) const noexcept {
    return images_[image_type];
  }

  ~ImageManager() noexcept { atexit(IMG_Quit); }
};

#endif
