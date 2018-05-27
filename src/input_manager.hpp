#ifndef INPUT_H
#define INPUT_H

#include "def_global.hpp"

namespace input_device {

enum {
  up = 0,
  down,
  left,
  right,
  f,
  x,
  space,
  count,
};

}  // namespace input_device

class InputManager {
  SDL_Joystick *joystick_;
  bool press_key_[input_device::count];
  bool edge_key_[input_device::count];

 public:
  InputManager() noexcept {
    if (SDL_NumJoysticks() > 0) {
      joystick_ = SDL_JoystickOpen(0);
    }
  }

  inline void update() {
    bool new_press_key[input_device::count];
    for (int i = 0; i < input_device::count; ++i) {
      new_press_key[i] = false;
    }
    if (joystick_) {
      SDL_JoystickUpdate();
      new_press_key[input_device::f] =
          SDL_JoystickGetButton(joystick_, 0) == SDL_PRESSED;
      new_press_key[input_device::x] =
          SDL_JoystickGetButton(joystick_, 1) == SDL_PRESSED;
      new_press_key[input_device::up] =
          SDL_JoystickGetAxis(joystick_, 1) < -256;
      new_press_key[input_device::down] =
          SDL_JoystickGetAxis(joystick_, 1) > 256;
      new_press_key[input_device::left] =
          SDL_JoystickGetAxis(joystick_, 0) < -256;
      new_press_key[input_device::right] =
          SDL_JoystickGetAxis(joystick_, 0) > 256;
    }

    Uint8 *keys = SDL_GetKeyState(NULL);
    new_press_key[input_device::up] =
        (keys[SDLK_UP] == SDL_PRESSED) || (keys['k'] == SDL_PRESSED);
    new_press_key[input_device::down] =
        (keys[SDLK_DOWN] == SDL_PRESSED) || (keys['j'] == SDL_PRESSED);
    new_press_key[input_device::left] =
        (keys[SDLK_LEFT] == SDL_PRESSED) || (keys['h'] == SDL_PRESSED);
    new_press_key[input_device::right] =
        (keys[SDLK_RIGHT] == SDL_PRESSED) || (keys['l'] == SDL_PRESSED);
    new_press_key[input_device::f] = keys['f'] == SDL_PRESSED;
    new_press_key[input_device::x] = keys['x'] == SDL_PRESSED;
    new_press_key[input_device::space] = keys[SDLK_SPACE] == SDL_PRESSED;
    for (int i = 0; i < input_device::count; ++i) {
      edge_key_[i] = !press_key_[i] && new_press_key[i];
      press_key_[i] = new_press_key[i];
    }
  }

  inline bool edge_key_p(const unsigned char button) const noexcept {
    return edge_key_[button];
  }

  inline bool press_key_p(const unsigned char button) const noexcept {
    return press_key_[button];
  }

  ~InputManager() noexcept {
    if (joystick_) {
      SDL_JoystickClose(joystick_);
    }
  }
};

#endif
