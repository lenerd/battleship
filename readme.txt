Requirements
* OpenSSL
* ncurses
* GMP
* Boost
* GTest (optional)

Build Requirements
* CMake
* Clang (optional)

Arch Linux

    # pacman -S openssl gmp ncurses boost boost-libs base-devel cmake clang gtest

Debian:

    # apt install libssl-dev libgmp-dev libncurses-dev libboost-all-dev build-essential cmake clang googletest



How to build:

1. Create build directory

    $ mkdir build
    $ cd build

2. Use CMake

    $ CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release ..

    or for debug symbols and address sanitizer:

    $ CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..

3. Build

    $ make


The following files should be available:
* battleship.exe
* test.exe
* web/
* battleship-prefix/src/battleship-build/test_proof.exe
