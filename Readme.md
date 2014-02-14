pcompiler (Precedence Compiler)
===============================

pcompiler is an automatic source code compiler.
Given a set of input files, pcompiler will attempt to create an executable out of them.

pcompiler is not robust enough to compile large or complex projects, but is extremely simple to use.

Requirements
============

* Qt 5.0 or higher
* CMake 2.8.12

Building (OS X and Linux)
=========================

	cd pcompiler
	mkdir build
	cd build
	cmake ..
	make
	make install

Authors
=======

* Braden McDorman
* Nafis Zaman

License
=======
pcompiler is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
