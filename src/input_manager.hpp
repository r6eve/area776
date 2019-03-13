//
//           Copyright r6eve 2019 -
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>
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
  SDL_Joystick *joystick_ = nullptr;
  bool press_key_[input_device::count];
  bool edge_key_[input_device::count];

 public:
  InputManager() noexcept {
    const int n = SDL_NumJoysticks();
    if (n < 0) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    if (n == 0) {
      // JoySticks not found.
      return;
    }

    // Only use one of JoySticks.
    joystick_ = SDL_JoystickOpen(0);
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

    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    new_press_key[input_device::up] = (state[SDL_SCANCODE_UP] == SDL_PRESSED) ||
                                      (state[SDL_SCANCODE_K] == SDL_PRESSED);
    new_press_key[input_device::down] =
        (state[SDL_SCANCODE_DOWN] == SDL_PRESSED) ||
        (state[SDL_SCANCODE_J] == SDL_PRESSED);
    new_press_key[input_device::left] =
        (state[SDL_SCANCODE_LEFT] == SDL_PRESSED) ||
        (state[SDL_SCANCODE_H] == SDL_PRESSED);
    new_press_key[input_device::right] =
        (state[SDL_SCANCODE_RIGHT] == SDL_PRESSED) ||
        (state[SDL_SCANCODE_L] == SDL_PRESSED);
    new_press_key[input_device::f] = state[SDL_SCANCODE_F] == SDL_PRESSED;
    new_press_key[input_device::x] = state[SDL_SCANCODE_X] == SDL_PRESSED;
    new_press_key[input_device::space] =
        state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
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
