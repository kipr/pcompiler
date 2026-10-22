# pcompiler (Precedence Compiler)

pcompiler is an automatic source code compiler.
Given a set of input files, pcompiler will attempt to create an executable out of them.

pcompiler is not robust enough to compile large or complex projects, but is extremely simple to use.

# Requirements

* Qt 5.0 or higher
* CMake 2.8.12
* libkar (only for the command line pcompiler tool)

# Building

## Cross-compile to the Wombat (Raspberry Pi 3b+)

Local build, tested on Debian 13:

```bash
sudo dpkg --add-architecture arm64
sudo apt update
sudo apt install make cmake gcc-aarch64-linux-gnu g++-aarch64-linux-gnu qt6-base-dev:arm64
# Replace with your version of `libkar`
sudo apt install ./libkar-1.0.1-Linux.deb
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
```

Build in the devcontainer: see [.devcontainer/README.md](.devcontainer/README.md).

Build with Docker:

```bash
docker build -t pcompiler-builder .
docker run --rm --mount type=bind,source=.,destination=/src/ pcompiler-builder sh -c 'cmake -B/src/build -DCMAKE_TOOLCHAIN_FILE=/src/toolchain/aarch64-linux-gnu.cmake /src && cmake --build /src/build -j "$(nproc)" && cd /src/build && cpack'
```

## OS X and Linux
```bash
	#Generate Library
	cd pcompiler
	mkdir build
	cd build
	cmake .. or cmake -Ddocker_cross=ON .. (docker cross compiling option)
	make
	make install

	#Install Generated Library
	cd .. #pcompiler root directory
	sudo cp lib/libpcompiler.so /usr/lib
```
## Windows

Prerequisite: The binaries/includes/libraries are installed into `<dir>\prefix`

1. Clone this repository into `<dir>\pcompiler`.
2. Configure it and generate the makefiles with cmake. Set the build directory to `<dir>\pcompiler\build`.
3. Open `<dir>\pcompiler\build\pcompiler.sln` with Visual Studio
4. Build the `INSTALL` project

The binaries/includes/libraries are installed into `<dir>\prefix`

Authors
=======

* Braden McDorman
* Nafis Zaman
* Erin Harrington

License
=======
pcompiler is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
