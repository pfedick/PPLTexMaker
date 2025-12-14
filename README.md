# PPLib Texture generator

## About
This is a tool to generate compined texture files from single images (sprites).




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


