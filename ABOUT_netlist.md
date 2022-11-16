OVERVIEW
--------

This directory provides a "SystemC netlist utility" to aid debug of SystemC designs. The idea to provide an easy to read hierarchical outline of your design in terms of module, channels, ports, processes and so forth.

The `netlist::print_hierarchy` function does most of the work (top-down visits every `sc_object`. The `netlist::object_info` method determines what to display for each object.

USAGE
-----

There are only two files needed:

1. netlist.h
2. netlist.cpp

All other files are related to lightweight testing.

1. Add `netlist.cpp` to the list source files to be compiled.
2. `#include "netlist.h"` in one of your modules (e.g., `Top_module`)
3. Call `netlist::print();` from `end_of_elaboration` or later. Alternately, use `netlist::print_from_top`
4. To suppress use of BOOST, add `-DNOBOOST` to compile flags when compiling `netlist.cpp`.

Note that print allows for an optional `std::ostream` to be specified in case you want to direct output to a file. Defaults to `std::cout`.

DESCRIPTION
-----------

This is not a full netlist yet, and may never be quite what we might desire. It lists a hierarchical tree of `sc_object`s and when possible determines connections from `port`s, `analysis_port`s, and `tlm::initiator_socket`s to their logical connections. Because of the way SystemC routes the connections, they show the final destination and not the in-between parts. This utility relies on polymorphism and RTTI. Currently, SystemC itself does not provide all the required machinery, and so several patches have been applied. If you apply the patches, you will get a slightly more comprehensive listing.

DIRECTORY LAYOUT
----------------

```
  ${WORKTREE_DIR}/
  ├── bin/ -- useful for building systemc and running cmake
  ├── cmake/ -- scripts used by cmake
  ├── CMakeLists.txt -- specifies files to compile and run
  ├── externs/ -- empty, used for certain external 3rd party installs
  ├── src/ -- source code for the project
  └── tests/ -- test source code
```


PATCHING SYSTEMC
----------------

The SystemC patches should not break anything and are quite limited. At present, the patches have only been applied to the Accellera Official repository and compiled using the "experimental" cmake approach, which I find to be simpler and more stable. 

Obtain a copy of my latest patches from <https://github.com/dcblack/systemc-patches.git>

Here are the steps:

1. Use the bin/build-systemc utility to configure, compile and install SystemC. Source is created under `"${HOME}/.local/src" and the compiled version is installed under `"${HOME}/.local/apps/systemc"`.
```bash
${NETLIST_HOME}/bin/build-systemc
```

2. Go to the `"${HOME}/.local/src"` directory and do the following:

```bash
git am --empty=drop "${NETLIST_HOME}/netlist.patches"
cmake -B build
cmake --build build
cmake --install build
```

TESTING
-------

```bash
HERE=`pwd`
cmake -S . -B build/debug -DCMAKE_PREFIX_PATH=$HERE -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
ctest --test-dir build/debug -V
```

REQUEST
-------

If you use this utility, please let us know of any issues. It would be important to know what platform OS/version and compiler/version you are using. If you make changes (improvements/bug-fixes/extensions), please let us know.

COPYRIGHT & LICENSE
-------------------

Copyright 2022 David C Black <david.black@doulos.com>

Licensed under Apache 2.0 -- See LICENSE file or <http://www.apache.org/licenses/LICENSE-2.0> for details.

vim:tw=80:wrap:nospell
