area776
=======
[![Build Status on Travis CI][]][Travis CI Results]
[![Build Status on AppVeyor][]][AppVeyor Results]

`area776` is a mini shooting game written in C++ with SDL1.2.

## Demo Video

[![Demo Image][]][Demo Video]

## Installation

```console
# Arch
> pacman -S sdl sdl_image sdl_mixer sdl_ttf
# Ubuntu
> apt-get -y install libsdl-dev libsdl-image-dev libsdl-mixer-dev libsdl-ttf-dev

# macOS
> brew update
> brew install sdl sdl_image sdl_mixer sdl_ttf

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

* Shoot: F Key, UP: Up Arrow, LEFT: Left Arrow, DOWN: Down Arrow, RIGHT: Right Arrow

## Requirement

* g++ / clang++
* [SDL 1.2][]
* [SDL_image 1.2][]
* [SDL_mixer 1.2][]
* [SDL_ttf for SDL 1.2][]

## Used Resources

* [源暎ゴシックP][]
* [フリーBGM・音楽素材 MusMus][]
* [ザ・マッチメイカァズ][]
* [効果音ラボ][]

## TODO

* Support SDL2.0
* Record Score

[Build Status on Travis CI]: https://travis-ci.org/r6eve/area776.svg?branch=master
[Travis CI Results]: https://travis-ci.org/r6eve/area776
[Build Status on AppVeyor]: https://ci.appveyor.com/api/projects/status/saw2lu2yrqn0omgs?svg=true
[AppVeyor Results]: https://ci.appveyor.com/project/r6eve/area776
[Demo Image]: http://img.youtube.com/vi/dYCtreFwPDk/0.jpg
[Demo Video]: https://youtu.be/dYCtreFwPDk
[SDL 1.2]: http://www.libsdl.org/
[SDL_image 1.2]: https://www.libsdl.org/projects/SDL_image/release-1.2.html
[SDL_mixer 1.2]: https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
[SDL_ttf for SDL 1.2]: https://www.libsdl.org/projects/SDL_ttf/release-1.2.html
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[フリーBGM・音楽素材 MusMus]: http://musmus.main.jp/
[ザ・マッチメイカァズ]: http://osabisi.sakura.ne.jp/m2/tm4/se_001.html
[効果音ラボ]: https://soundeffect-lab.info/
