area776
=======

*TODO PROJECT*

`area776` is a shooting game written in C++ with SDL1.2.

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

> git clone git@github.com:r6eve/area776.git  # Or download ZIP archive and extract pacman-sdl.
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

* Shoot: Z Key, UP: Up Arrow, LEFT: Left Arrow, DOWN: Down Arrow, RIGHT: Right Arrow

## Requirement

* g++ / clang++
* [SDL 1.2][]
* [SDL_image 1.2][]
* [SDL_mixer 1.2][]
* [SDL_ttf for SDL 1.2][]

## Used Resources

* [源暎ゴシックP][]
* [フリーBGM・音楽素材][]
* [ザ・マッチメイカァズ][]

## TODO

* Do Refactoring
* Support SDL2.0
* Record Score

[SDL 1.2]: http://www.libsdl.org/
[SDL_image 1.2]: https://www.libsdl.org/projects/SDL_image/release-1.2.html
[SDL_mixer 1.2]: https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
[SDL_ttf for SDL 1.2]: https://www.libsdl.org/projects/SDL_ttf/release-1.2.html
[源暎ゴシックP]: https://okoneya.jp/font/genei-antique.html
[フリーBGM・音楽素材 MusMus]: http://musmus.main.jp/
[ザ・マッチメイカァズ]: http://osabisi.sakura.ne.jp/m2/tm4/se_001.html
