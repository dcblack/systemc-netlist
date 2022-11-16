#pragma once

#include <systemc>
#include <iostream>
#include <vector>
#include <string>

namespace netList {

  //----------------------------------------------------------------------------
  // Find the topmost sc_object from a specified one.
  sc_core::sc_object* get_top( sc_core::sc_object* obj_ptr );

  //----------------------------------------------------------------------------
  // Returns a string describing the object
  std::string object_info( sc_core::sc_object* obj_ptr, ssize_t depth, bool fullpath=false );

  //----------------------------------------------------------------------------
  // Dump netList from specified object
  void print_hierarchy( const std::vector<sc_core::sc_object*>& children
                 , ssize_t depth
                 , std::ostream& os = std::cout );

  //----------------------------------------------------------------------------
  // Dump netList from top of specified object
  void print_from_top( sc_core::sc_object* obj_ptr
                     , std::ostream& os = std::cout );

  //----------------------------------------------------------------------------
  // Dump entire netList to stdout from all tops
  void print( std::ostream& os = std::cout );

  #ifdef TODO /* Untested and partially started ideas */
  //----------------------------------------------------------------------------
  // Search for module containing the current process
  sc_object* process_top()

  //----------------------------------------------------------------------------
  // Returns a collection of objects matching the regular expression.  Note that
  // a forward leaning slash (/) becomes a literal period ('.') when searching
  // to simplify the syntax.
  std::vector<sc_core::sc_object*> search( const std::string& regex );
  #endif

}

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
