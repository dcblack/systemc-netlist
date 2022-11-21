Summary
-------

See `ABOUT_netlist.md` for details.

This directory provides a "SystemC netlist utility" to aid debug of SystemC designs. The idea to provide an easy to read hierarchical outline of your design in terms of module, channels, ports, processes and so forth.

Contents
--------

```
├── ABOUT_netlist.md -- This file
├── CMakeLists.txt -- Used to build (configure, compile, link, and test)
├── LICENSE
├── NOTICE
├── README.md
├── bin/ -- scripts (see ABOUT_bin.md enclosed)
├── cmake/ -- support files
├── externs/ -- empty
├── setup.profile -- bash script sourced to setup environment
├── src/ -- the stuff you probably want to use
│   ├── netlist.cpp
│   └── netlist.h
└── tests/ -- a bunch of files to test the utility


