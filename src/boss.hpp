#ifndef BOSS_H
#define BOSS_H

#include "def_global.hpp"
#include "image_manager.hpp"
#include "mixer_manager.hpp"
#include "vector.hpp"

void init_boss();
void move_boss(MixerManager &mixer_manager);
void move_boss_shot();

/**
 * Return true if boss life is 0.
 */
bool check_myshots_hit_boss();

void draw_boss(SDL_Surface *screen, ImageManager &image_manager);
void draw_boss_shot(SDL_Surface *screen, ImageManager &image_manager);

#endif
