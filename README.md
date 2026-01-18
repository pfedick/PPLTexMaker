# PPLib Texture generator

## About
This is a tool to generate texture files from single images (sprites). The files can be used to implement Sprites in 2D games.

It automatically detects the borders of a sprite and stores only the pixels within this border. Alle sprites will get sorted by size and placed in an optimal way on the textures, to save space (and RAM). In addition, a lookup table is generated and stored in the texture file, too. The lookup table is used to find the texture and position of a specific sprite. Offset and pivot point of the sprite is also part of the lookup table.

Beside the opaque pixels (albedo), normal and sepecular maps can be stored, too.

## How to use

The tool expects, that all sprites are stored in a directory. If you use normal maps or specular maps, they have to be stored in their own directories. Alls Sprites must be stored as png-Files. Filenames get sorted alphabetically and the "lowest" filename will be sprite number 0. Filenames of normal oder specular maps must be identic to the albedo sprites.

When using the "-i"-Option, it is expected, that the filename contains a number, which is then used as index in the Texture-File. Example: "frame_0042.png" will get Index 42. It is also possible, to code two numbers into the filename. Both numbers will then be added together to get the index. Example: "frame_1000_0042.png" will get Index 1042. "1000_42.png" will also work.



File structure:

When using albedo only, you can put all sprites in one directory:

```
+-- base directory
    +-- frame_0000.png
    +-- frame_0001.png
    +-- ...
```

When using normal or specular maps, you have to use this structure:

```
+-- base directory
    |
    +-- albedo
        +-- frame_0000.png
        +-- frame_0001.png
        +-- ...
    +-- normal
        +-- frame_0000.png
        +-- frame_0001.png
        +-- ...
    +-- specular
        +-- frame_0000.png
        +-- frame_0001.png
        +-- ...
```

Todo
```bash
./texmaker -s SOURCE_DIRECTORY -t TARGETFILE ...

```

Options:

| Option      | Description        |
|-----------|-------------|
| -s SOURCE | source directory |
| -t TARGET | target file |
| -f FILE   | json or yaml file with sprite data (optional) |
| -w PIXEL  | width of textures |
| -h PIXEL  | height of textures |
| -i        | take index from filename |
| -pd METHOD | Method of pivot detection: <br>- lower_left<br>- lower_middle (=default)<br>- fixed (use -px and -py)|
| -px PIXEL | x-position of pivot point when using -pd fixed |
| -py PIXEL | y-position of pivot point when using -pd fixed |
| -a AUTHOR | Optional, Name of Author (use quotes when using spaces) |
| -c COPYRIGHT | Optional, Copyright String (use quotes when using spaces) |
| -n NAME | Optional, Name of Sprites (use quotes when using spaces) |
| -d DESCRIPTION | Optional, Description (use quotes when using spaces) |
| -x FILE | Optional, saves every albedo texture as png-file with this prefix |
| --help | Shows help |


## Optional Config-File
All Values in this file are optional
If you have a single sprite with an alternate pivot point, then it is possible to configure only this single sprite in the config file


### Json-Format

```json
{
  "source": "SOURCE_DIRECTORY",
  "target": "TARGETFILE",
  "width": 2048, // width of textures
  "height": 2048, // height of textures
  "pivot": {
    "method": "METHOD", // lower_left, lower_middle (=default), fixed
    "x": 64, // x-position of pivot point when using method fixed
    "y": 128, // y-position of pivot point when using method fixed
  },
  "author": "AUTHOR",
  "copyright": "COPYRIGHT",
  "name": "NAME",
  "description": "DESCRIPTION",

  "sprites": [
    {
      "index": 1,
      "albedo": "FILENAME",
      "normal": "FILENAME",
      "specular": "FILENAME",
      "pivot": {
        "method": "lower_middle"
      }
    },
    {
      "index": 2,
      "albedo": "FILENAME",
      "normal": "FILENAME",
      "specular": "FILENAME",
      "pivot": {
        "method": "fixed",
        "x": 0,
        "y": 0
      }
    },
    ...
  ]
}
```

### Yaml-Format (TODO, not supported yet)

```yaml
---
source: SOURCE_DIRECTORY
target: TARGETFILE
width: 2048 # width of textures
height: 2048 # height of textures
pivot:
  method: METHOD # lower_left, lower_middle (=default), fixed
  x: x-position of pivot point when using method fixed
  y: y-position of pivot point when using method fixed
author: AUTHOR
copyright: COPYRIGHT
name: NAME
description: DESCRIPTION

sprites:
  - index: 1
    albedo: FILENAME
    normal: FILENAME
    specular: FILENAME
    pivot:
      method: lower_middle
  - index: 2
    albedo: FILENAME
    normal: FILENAME
    specular: FILENAME
    pivot:
      method: fixed
      x: 0
      y: 0    
  ...
```


## PPLIB (aka ppl7)
This projects uses my library pplib as abstraction layer to the underlaying operation system, strings and graphic functions.

See: https://github.com/pfedick/pplib

pplib is a submodule in this project and can be found in the pplib direcory. Header files can be found in pplib/include




## How to compile
### Requirements
The following Tools/Libraries are required:
- gcc or clang with c++ support (C++11 or greater)
- gnu make
- pcre2
- iconv
- zlib
- bz2
- png

#### Fedora
```bash
sudo dnf install -y git gcc gcc-c++ make pcre2-devel zlib-devel \
    bzip2-devel libpng-devel libjpeg-turbo-devel
```

#### Ubuntu
```bash
sudo apt-get install -y git gcc g++ libpcre2-dev libbz2-dev zlib1g-dev \
    libpng-dev 
```

#### Windows 10 with mingw64/msys

For installing msys2 and mingw64, please see: [https://www.msys2.org/](https://www.msys2.org/)

```bash
pacman -S git make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-zlib \
    mingw-w64-ucrt-x86_64-libiconv mingw-w64-ucrt-x86_64-bzip2 \
    mingw-w64-ucrt-x86_64-pcre2 mingw-w64-ucrt-x86_64-libpng \
    mingw-w64-ucrt-x86_64-libjpeg-turbo
    pkgconf
```

#### FreeBSD

```bash
pkg install pcre2 bzip2 png jpeg-turbo pkgconf gmake
```

Use "gmake" instead of "make" for compiling below!

### Checkout and compile

```bash
git clone --recurse-submodules https://github.com/pfedick/PPLTexMaker.git
cd PPLTexMaker
./configure
make
# for faster compile you can add the paraneter "-j CORES", where CORES is the number
# of cores on you cpu:
make -j 8

# start the tool
./texmaker
```

If you already cloned the project and want to update to the newest version:

```bash
git pull
git submodule update --init --recursive
./configure
make
```


## Format der Textur-Datei 

Please see: [format.md](format.md)



