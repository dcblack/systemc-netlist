#pragma once

#include <tlm>
#include <string>
#include "sc_time_literals.hpp"
using namespace sc_core::literals;
using namespace sc_core;
using namespace tlm;
using namespace std::literals;

struct Dump_subscriber : sc_core::sc_object, tlm_analysis_if<std::string> {
  static constexpr const char *mesgType = "/Doulos Inc./SystemC-Example/Dump";
  explicit Dump_subscriber( const char* nm ) : sc_object{nm} {
    SC_REPORT_INFO_VERB( mesgType, ("Contructed "s + name()).c_str(), SC_DEBUG );
  }
  const char* kind() const override { return "tlm_analysis_subscriber"; }
  void write( const std::string& t ) override { SC_REPORT_INFO_VERB( mesgType, ( "Dump: "s + t).c_str() , SC_DEBUG ); }
};

SC_MODULE( Empty_module )
{
  static constexpr const char *mesgType = "/Doulos Inc./SystemC-Example/Empty";
  SC_CTOR( Empty_module ) {
    SC_REPORT_INFO_VERB( mesgType, ("Contructed "s + name()).c_str() , SC_DEBUG );
  }
};

SC_MODULE( Odd_module )
{
  static constexpr const char *mesgType = "/Doulos Inc./SystemC-Example/top2";
  // Convenience & readability
  template<typename T>
  using dangleOut_port = sc_core::sc_port<sc_core::sc_signal_out_if<T>, 1, SC_ZERO_OR_MORE_BOUND>;
  template<typename T>
  using inout_export = sc_core::sc_export<sc_core::sc_signal_inout_if<T>>;
  template<typename T>
  using multi_port_t = sc_core::sc_port<sc_core::sc_signal_out_if<T>, 0, SC_ZERO_OR_MORE_BOUND>;
  // Ports & Channels
  dangleOut_port<bool>            dangling{ "dangling" };
  sc_core::sc_inout<bool>         nameless_port{};
  inout_export<bool>              nameless_xport   {};
  sc_core::sc_signal<bool>        anonymous_channel{};
  multi_port_t<int>               multi_port { "multi_port" };
  tlm_analysis_port<std::string>  debug_ap{ "debug_ap" };
  tlm_analysis_port<int>          anon_ap{};
  sc_core::sc_attribute<int>      attr_int{ "attr_int"};
  SC_CTOR( Odd_module )
  {
    SC_THREAD( hello2 );
    nameless_xport.bind( anonymous_channel );
    SC_REPORT_INFO_VERB( mesgType, ("Contructed "s + name()).c_str() , SC_DEBUG);
  }

  void hello2()
  {
    wait( 1_us );
    auto message = "Hello2 @ "s + sc_time_stamp().to_string();
    debug_ap.write( message );
    SC_REPORT_INFO( mesgType, message.c_str() );
  }
};

SC_MODULE( Container_module ) {
  const char* mesgType = "/Doulos Inc./SystemC-Example/top2";
  sc_core::sc_signal<int>  firstOfTwo  { "firstOfTwo" };
  sc_core::sc_signal<int>  secondOfTwo { "secondOfTwo" };
  sc_mutex                 mutex       { "mutex" };
  Odd_module               odd1        { "odd1" };
  Empty_module             empty       { "empty" };
  Odd_module               odd2        { "odd2" };
  Dump_subscriber          dumper      { "dumper" };
//tlm_analysis_fifo<int>   analysis    { "analysis" };
  SC_CTOR( Container_module )
  {
    odd1.multi_port.bind( firstOfTwo );
    odd1.multi_port.bind( secondOfTwo );
    odd1.nameless_port.bind( odd2.nameless_xport );
    odd2.nameless_port.bind( odd1.nameless_xport );
    odd1.debug_ap.bind( dumper );
//  odd2.anon_ap.bind( analysis );
    SC_REPORT_INFO_VERB( mesgType, ("Contructed "s + name()).c_str() , SC_DEBUG );
  }
};

SC_MODULE( Top2_module ) {
  const char* mesgType = "/Doulos Inc./SystemC-Example/top2";
  Container_module container{ "container" };
  SC_CTOR( Top2_module ) {
    SC_REPORT_INFO_VERB( mesgType, ("Contructed "s + name()).c_str() , SC_DEBUG );
  }
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
