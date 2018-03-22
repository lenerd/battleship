# Battleship

This is an implementation of the battleship game that I wrote for my bachelor's thesis. It employs a cryptographic protocol based on commitments and secure two-party computation to ensure that cheating behavior is detected without requiring a trusted third party.


## Dependencies
The following dependencies are referenced as Git submodules.

* [ABY (AGPL-v3.0)](https://github.com/encryptogroup/ABY)
	* [OTExtension (AGPL-3.0)](https://github.com/encryptogroup/OTExtension)
	* [ENCRYPTO_utils (AGPL-3.0)](https://github.com/encryptogroup/ENCRYPTO_utils)
	* [MIRACL (AGPL-3.0)](https://github.com/miracl/MIRACL)
* [Crypto++ (BSL-1.0)](https://github.com/weidai11/cryptopp)
	* [Crypto++ CMake (public domain)](https://github.com/noloader/cryptopp-cmake)
* [json (MIT)](https://github.com/nlohmann/json)
* [flatbuffers (Apache-2.0)](https://github.com/google/flatbuffers)
* [Beast (BSL-1.0)](https://github.com/boostorg/beast)

Other dependencies that are usually available via the package manager are listed in the build section.

## Building

### Requirements
* OpenSSL
* ncurses
* GMP
* Boost
* GTest (optional)
* CMake (for the build)
* Clang (for the build, optional)

Arch Linux

    # pacman -S openssl gmp ncurses boost boost-libs base-devel cmake clang gtest

Debian:

    # apt install libssl-dev libgmp-dev libncurses-dev libboost-all-dev build-essential cmake clang googletest


### How to build:

1. Clone the repository

	    $ git clone --recursive $CLONE_URL battleship

2. Create build directory

        $ mkdir -p battleship/build
        $ cd battleship/build

3. Use CMake

          CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release ..

    or for debug symbols and address sanitizer:

        $ CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..

4. Build

        $ make


After the build is complete, the following files should be available:
* `battleship.exe`
* `test.exe`
* `web/`
* `battleship-prefix/src/battleship-build/test_proof.exe`
