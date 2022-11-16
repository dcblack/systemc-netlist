#!cmake .
cmake_minimum_required ( VERSION 3.20 )

# Header style guard for multiple inclusion protection
if( DEFINED PROJECT_DEFAULTS )
  return()
endif()
set( PROJECT_DEFAULTS ON )

message( "Setting up cmake" )

#-------------------------------------------------------------------------------
# Extend module path
#-------------------------------------------------------------------------------
if( DEFINED ENV{WORKTREE_DIR} )
  set( PROJECT_DIRS "$ENV{WORKTREE_DIR}/externs;$ENV{APPS}" )
  foreach( _dir ${PROJECT_DIRS} )
     if( EXISTS "${_dir}" )
       list( APPEND CMAKE_PREFIX_PATH "${_dir}" )
     endif()
  endforeach()
  list( REMOVE_DUPLICATES CMAKE_PREFIX_PATH )
  include_directories( . "$ENV{WORKTREE_DIR}/include" )
else()
  message( WARNING "WORKTREE_DIR environment variable was not defined" )
endif()

#-------------------------------------------------------------------------------
# Increase sensitivity to all warnings
#-------------------------------------------------------------------------------
if( DEFINED ENV{LAX} )
  include( course )
else()
  include( strict )
endif()

#------------------------------------------------------------------------------
# Choose minimum C++ standard for remainder of code
#------------------------------------------------------------------------------
if( NOT DEFINED USE_CXX_VERSION )
  set( USE_CXX_VERSION 17 )
endif()
if( NOT DEFINED USE_C_VERSION )
  set( USE_C_VERSION 11 )
endif()
set( CMAKE_CXX_STANDARD          ${USE_CXX_VERSION} CACHE STRING "C++ standard to build all targets." )
set( CMAKE_CXX_STANDARD_REQUIRED ${USE_CXX_VERSION} CACHE BOOL   "The CMAKE_CXX_STANDARD selected C++ standard is a requirement." )
set( CMAKE_C_STANDARD            ${USE_C_VERSION}   CACHE STRING "C standard to build all targets." )
set( CMAKE_C_STANDARD_REQUIRED   ${USE_C_VERSION}   CACHE BOOL   "The CMAKE_CXX_STANDARD selected C standard is a requirement." )

enable_testing()

include( set_target )

# vim:nospell
