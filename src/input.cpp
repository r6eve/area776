#include "def_global.hpp"

static SDL_Joystick *Joystick;

void init_joystick() {
  if (SDL_NumJoysticks() > 0) {
    Joystick = SDL_JoystickOpen(0);
  }
}

void update_input() {
  bool new_press_key[NUM_PRESS_KEY];
  for (int i = 0; i < NUM_PRESS_KEY; ++i) {
    new_press_key[i] = false;
  }
  if (Joystick) {
    SDL_JoystickUpdate();
    if (SDL_JoystickGetButton(Joystick, 0) == SDL_PRESSED) {
      new_press_key[PRESS_KEY_BUTTON_0] = true;
    }
    if (SDL_JoystickGetButton(Joystick, 1) == SDL_PRESSED) {
      new_press_key[PRESS_KEY_BUTTON_1] = true;
    }
    if (SDL_JoystickGetAxis(Joystick, 1) < -256) {
      new_press_key[PRESS_KEY_UP] = true;
    }
    if (SDL_JoystickGetAxis(Joystick, 1) > 256) {
      new_press_key[PRESS_KEY_DOWN] = true;
    }
    if (SDL_JoystickGetAxis(Joystick, 0) < -256) {
      new_press_key[PRESS_KEY_LEFT] = true;
    }
    if (SDL_JoystickGetAxis(Joystick, 0) > 256) {
      new_press_key[PRESS_KEY_RIGHT] = true;
    }
  }

  Uint8 *keys = SDL_GetKeyState(NULL);
  if ((keys[SDLK_UP] == SDL_PRESSED) || (keys['k'] == SDL_PRESSED)) {
    new_press_key[PRESS_KEY_UP] = true;
  }
  if ((keys[SDLK_DOWN] == SDL_PRESSED) || (keys['j'] == SDL_PRESSED)) {
    new_press_key[PRESS_KEY_DOWN] = true;
  }
  if ((keys[SDLK_LEFT] == SDL_PRESSED) || (keys['h'] == SDL_PRESSED)) {
    new_press_key[PRESS_KEY_LEFT] = true;
  }
  if ((keys[SDLK_RIGHT] == SDL_PRESSED) || (keys['l'] == SDL_PRESSED)) {
    new_press_key[PRESS_KEY_RIGHT] = true;
  }
  if (keys['z'] == SDL_PRESSED) {
    new_press_key[PRESS_KEY_BUTTON_0] = true;
  }
  if (keys['x'] == SDL_PRESSED) {
    new_press_key[PRESS_KEY_BUTTON_1] = true;
  }
  if (keys[SDLK_SPACE] == SDL_PRESSED) {
    new_press_key[PRESS_KEY_SPACE] = true;
  }
  for (int i = 0; i < NUM_PRESS_KEY; ++i) {
    Edge_key[i] = false;
    if ((!Press_key[i]) && new_press_key[i]) {
      Edge_key[i] = true;
    }
    Press_key[i] = new_press_key[i];
  }
}

void end_joystick() {
  if (Joystick) {
    SDL_JoystickClose(Joystick);
  }
}
