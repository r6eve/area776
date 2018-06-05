#ifndef ENEMY_H
#define ENEMY_H

#include "def_global.hpp"
#include "fighter.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"

class Enemies {
  int life_;

 public:
  class Enemy {
    bool view_p_;
    Point pos_;
    Point move_;
    int shot_timer_;

   public:
    Enemy() noexcept {}

    inline void init() noexcept { view_p_ = false; }

    inline void draw(SDL_Renderer *renderer,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }

      SDL_Texture *enemy_texture = image_manager.get(renderer, image::mons13);
      SDL_Rect dst;
      dst.x = static_cast<Sint16>(pos_.x);
      dst.y = static_cast<Sint16>(pos_.y);
      SDL_QueryTexture(enemy_texture, nullptr, nullptr, &dst.w, &dst.h);
      SDL_Rect src;
      src.x = 0;
      src.y = 0;
      src.w = dst.w;
      src.h = dst.h;
      SDL_RenderCopy(renderer, enemy_texture, &src, &dst);
      SDL_DestroyTexture(enemy_texture);
    }

    /**
     * Return false if updated enemy is invisible or shot timer is not 0, and
     * true otherwise.
     */
    inline bool update() noexcept {
      if (!view_p_) {
        return false;
      }

      pos_ += move_;
      if (pos_.x < -35) {
        view_p_ = false;
        return false;
      }
      if (pos_.x > screen::width) {
        view_p_ = false;
        return false;
      }
      if (pos_.y > screen::height) {
        view_p_ = false;
        return false;
      }

      --shot_timer_;
      if (shot_timer_ != 0) {
        return false;
      }

      return true;
    }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_visible(const Fighter &fighter) noexcept {
      view_p_ = true;
      pos_.x = rand() % (screen::width - 64);
      pos_.y = -64;
      move_ = fighter.get_pos() - pos_;
      move_.norm();
      move_ *= 8;
      shot_timer_ = rand() % 15 + 15;
    }

    inline void make_invisible() noexcept { view_p_ = false; }

    inline Point get_pos() const noexcept { return pos_; }

    ~Enemy() noexcept {}
  };

  class Bullet {
    bool view_p_;
    Point pos_;
    Point move_;

   public:
    Bullet() noexcept {}

    inline void init() noexcept { view_p_ = false; }

    inline void draw(SDL_Renderer *renderer,
                     const ImageManager &image_manager) const noexcept {
      if (!view_p_) {
        return;
      }

      SDL_Texture *bullet_texture = image_manager.get(renderer, image::bm01);
      SDL_Rect dst;
      dst.x = static_cast<Sint16>(pos_.x);
      dst.y = static_cast<Sint16>(pos_.y);
      SDL_QueryTexture(bullet_texture, nullptr, nullptr, &dst.w, &dst.h);
      SDL_Rect src;
      src.x = 0;
      src.y = 0;
      src.w = dst.w;
      src.h = dst.h;
      SDL_RenderCopy(renderer, bullet_texture, &src, &dst);
      SDL_DestroyTexture(bullet_texture);
    }

    inline void update() noexcept {
      if (!view_p_) {
        return;
      }

      pos_ += move_;
      if (pos_.x < -16) {
        view_p_ = false;
      }
      if (pos_.y < -16) {
        view_p_ = false;
      }
      if (pos_.x > screen::width) {
        view_p_ = false;
      }
      if (pos_.y > screen::height) {
        view_p_ = false;
      }
    }

    inline void shoot(const Point &enemy_center, const Point &move) noexcept {
      view_p_ = true;
      pos_ = enemy_center;
      move_ = move;
    }

    inline bool view_p() const noexcept { return view_p_; }

    inline void make_invisible() noexcept { view_p_ = false; }

    inline Point get_pos() const noexcept { return pos_; }

    ~Bullet() noexcept {}
  };

  Enemy enemies[ENEMY_MAX];
  Bullet bullets[ENEMY_SHOT_MAX];

  Enemies() noexcept {}

  inline void init(const bool debug_mode) noexcept {
    for (auto &enemy : enemies) {
      enemy.init();
    }
    for (auto &bullet : bullets) {
      bullet.init();
    }
    life_ = debug_mode ? 5 : 30;
  }

  inline void update(const MixerManager &mixer_manager,
                     const Fighter &fighter) noexcept {
    if (rand() % 45 == 0) {
      for (auto &enemy : enemies) {
        if (enemy.view_p()) {
          continue;
        }
        enemy.make_visible(fighter);
        break;
      }
    }

    for (auto &bullet : bullets) {
      bullet.update();
    }

    const double shot_pitch = 20;

    for (auto &enemy : enemies) {
      if (!enemy.update()) {
        continue;
      }

      const Point enemy_center = enemy.get_pos() + 32;
      const Point fighter_center = fighter.get_pos() + 32;
      Point p = fighter_center - enemy_center;
      p.norm();
      p *= 6;
      const double rot_angle = -(shot_pitch * 2) * PI / 180;
      p.rot(rot_angle);
      for (int _ = 0; _ < 5; ++_) {
        for (auto &bullet : bullets) {
          if (bullet.view_p()) {
            continue;
          }
          bullet.shoot(enemy_center, p);
          break;
        }
        const double rot_angle = shot_pitch * PI / 180;
        p.rot(rot_angle);
        Mix_PlayChannel(se_type::enemy_shoot,
                        mixer_manager.get_se(se_type::enemy_shoot), 0);
      }
    }
  }

  inline void draw(SDL_Renderer *renderer,
                   const ImageManager &image_manager) const noexcept {
    for (const auto &enemy : enemies) {
      enemy.draw(renderer, image_manager);
    }
    for (const auto &bullet : bullets) {
      bullet.draw(renderer, image_manager);
    }
  }

  inline int get_life() const noexcept { return life_; }

  inline void set_life(const int life) noexcept { life_ = life; }

  ~Enemies() noexcept {}
};

#endif
