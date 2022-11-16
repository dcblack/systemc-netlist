#pragma once

#include "stimulus.h"
#include "bus.hpp"
#include "response.h"
#include <systemc>
#include "sc_time_literals.hpp"
#include "netlist.h"
using namespace sc_core::literals;
using namespace std::literals;

SC_MODULE(Top_module) {
  static constexpr const char* mesgType = "/Doulos Inc/SystemC-Example/top";
  // typedefs
  using sc_clock = sc_core::sc_clock;
  template<typename T> using sc_fifo = sc_core::sc_fifo<T>;

  // Channels and modules
  sc_clock topClock{ "topClock", 10_ns };
  Stimulus_module stimulus{ "stimulus" };
  Bus_module      bus{ "bus" };
  Response_module response{ "response" };
  sc_fifo<int> response_fifo{ "response_fifo", 2 };

  SC_CTOR(Top_module) {
    for( auto i=1; i<sc_core::sc_argc(); ++i ) {
      if( "--debug"s != sc_core::sc_argv()[i] ) continue;
      sc_core::sc_report_handler::set_verbosity_level( sc_core::SC_DEBUG );
    }
    sc_core::sc_report_handler::set_actions( sc_core::SC_ERROR, sc_core::SC_DISPLAY | sc_core::SC_LOG );
    stimulus.init_socket.bind( bus.targ_socket );
    bus.init_socket.bind( response.targ_socket );
    stimulus.dataout_port.bind( response.datain_xport );
    stimulus.datain_port.bind( response_fifo );
    response.dataout_port.bind( response_fifo );
    stimulus.clockin_port.bind( topClock );
    response.clockin_port.bind( topClock );
    SC_REPORT_INFO( mesgType, ( "Constructed Top_module \""s + name() + "\"" ).c_str() );
  }
  void end_of_elaboration() override {
    SC_REPORT_INFO( mesgType, __func__ );
    netList::print();
  }
  ~Top_module() override = default; // required for std::unique_ptr<Top_module>
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
