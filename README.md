area776
=======
[![Build Status on GitHub Actions][]][GitHub Actions Results]
[![Build Status on AppVeyor][]][AppVeyor Results]

`area776` is a mini shooting game written in C++ with SDL2.0.

## Demo Video (Clickable)

[![Demo Image][]][Demo Video]

## Installation

```console
# Arch
> pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
# Ubuntu
> apt-get -y install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

# macOS
> brew update
> brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf

# Windows
# Cf. .appveyor.yml

# If you use others, install those from package manager or source.

> git clone git@github.com:r6eve/area776.git  # Or download ZIP archive and extract area776.
> cd area776
> make
```

## Usage

```console
> ./area776 --help
Usage: area776 [options]

Options:
    -d  --debug         debug mode
    -h, --help          print this help menu
    -v, --version       print version
> ./area776  # normal mode
> ./area776 -d  # debug mode
```

### Key Config

* Shoot: F Key
* UP, LEFT, DOWN, RIGHT: ↑ ← ↓ →

## Requirement

* g++ / clang++
* [SDL 2.0][]
* [SDL_image 2.0][]
* [SDL_mixer 2.0][]
* [SDL_ttf 2.0][]

## Used Resources

* [源暎ゴシックP][]
* [フリーBGM・音楽素材 MusMus][]
* [ザ・マッチメイカァズ][]
* [効果音ラボ][]

## TODO

* Record Score

[Build Status on GitHub Actions]: https://github.com/r6eve/area776/workflows/main/badge.svg
[GitHub Actions Results]: https://github.com/r6eve/area776/actions
[Build Status on AppVeyor]: https://ci.appveyor.com/api/projects/status/saw2lu2yrqn0omgs?svg=true
[AppVeyor Results]: https://ci.appveyor.com/project/r6eve/area776
[Demo Image]: http://img.youtube.com/vi/dYCtreFwPDk/0.jpg
[Demo Video]: https://youtu.be/dYCtreFwPDk
[SDL 2.0]: http://www.libsdl.org/
[SDL_image 2.0]: https://www.libsdl.org/projects/SDL_image/
[SDL_mixer 2.0]: https://www.libsdl.org/projects/SDL_mixer/
[SDL_ttf 2.0]: https://www.libsdl.org/projects/SDL_ttf/
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[フリーBGM・音楽素材 MusMus]: http://musmus.main.jp/
[ザ・マッチメイカァズ]: http://osabisi.sakura.ne.jp/m2/tm4/se_001.html
[効果音ラボ]: https://soundeffect-lab.info/
