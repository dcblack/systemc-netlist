#!cmake .
#
# This works with SystemC versions 2.3.2 and later, which have support for
# cmake and specify SystemC_CXX_STANDARD.
#
# REQUIREMENTS:
# - Adjust ../../setup.profile if needed

cmake_minimum_required( VERSION 3.20 )

project( netlist VERSION 6.0 DESCRIPTION "Example of how to obtain a netlist" LANGUAGES CXX )

list( PREPEND CMAKE_MODULE_PATH "$ENV{WORKTREE_DIR}/cmake" )
include( sc-project_defaults )

# Comment-out following if not using boost
find_package( Boost REQUIRED )

set_target( "${PROJECT_NAME}" )
add_executable( "${Target}" )
# Adjust following as needed (e.g., add NOBOOST and/or NOUTF8)
target_compile_definitions( "${Target}"
  PRIVATE
  SC_ENABLE_COVARIANT_VIRTUAL_BASE
)
target_include_directories( "${Target}"
  PRIVATE
  "${WORKTREE_DIR}/include"
  "${Boost_INCLUDE_DIRS}"
  src
  tests
)
target_link_libraries( "${Target}" PRIVATE "${Boost_LIBRARIES}")
target_sources( "${Target}"
  PUBLIC
  src/netlist.h
  src/netlist.cpp
  PRIVATE
  tests/stimulus.h
  tests/stimulus.cpp
  tests/bus.hpp
  tests/bus.cpp
  tests/operation.h
  tests/response.h
  tests/response.cpp
  tests/top.h
  tests/top2.h
  tests/cppversion.hpp
  tests/main.cpp
  )
add_test( NAME "${PROJECT_NAME}-debug" COMMAND "${PROJECT_NAME}" "--debug" )

# vim:syntax=cmake:nospell

# ****************************************************************************
# This file is licensed under Apache-2.0, and
# Copyright 2022 David C Black <david.black@doulos.com>
# See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
