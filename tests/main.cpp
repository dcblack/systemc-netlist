#include "top.h"
#include "top2.h"
#include "sc_time_literals.hpp"
#include "cppversion.hpp"
#include <memory>
using namespace sc_core::literals;
using namespace std::literals;
using namespace sc_core;

namespace {
  const char* mesgType = "/Doulos Inc./SystemC-Example/main";
  std::string count( size_t n ) {
    if( n == 0 ) return "No"s;
    else         return std::to_string( n );
  }
}

int sc_main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  SC_REPORT_INFO( mesgType, cppversion().c_str() );
  auto top = std::make_unique<Top_module>( "top" );
  Top2_module top2{ "top2" };
  sc_start( 1000_ns );
  if ( not sc_core::sc_end_of_simulation_invoked() ) {
    SC_REPORT_WARNING( mesgType, "sc_stop was not invoked!" );
    sc_stop(); // invokes end_of_simulation
  }
  // Summarize simulation results
  auto warningCount = sc_report_handler::get_count(SC_WARNING);
  auto errorCount   = sc_report_handler::get_count(SC_ERROR);
  auto fatalCount   = sc_report_handler::get_count(SC_FATAL);
  SC_REPORT_INFO( mesgType, ( "\n\nSUMMARY:\n  "s
      + "Finished at "s + sc_time_stamp().to_string() + "\n  "s
      + count( warningCount ) + " warning messages\n  "s
      + count( errorCount )   + " error messages\n  "s
      + count( fatalCount )   + " fatal messages\n  "s
      + "Simulation "s + (( errorCount + fatalCount )? "FAILED"s : "PASSED"s)
    ).c_str()
  );
  return ( errorCount + fatalCount ) ? 1 : 0;
}

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
