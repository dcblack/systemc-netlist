#include "stimulus.h"
#include "operation.h"
#include "objection.h"
#include "sc_time_literals.hpp"
#include <string>
using namespace std::literals;
using namespace tlm;
using namespace sc_core;
using namespace sc_core::literals;

Stimulus_module::Stimulus_module( const sc_module_name& nm )
: sc_module( nm )
{
  SC_HAS_PROCESS( Stimulus_module );
  SC_THREAD( stim_thread );
  SC_METHOD( stim_method );
    sensitive << clockin_port;
    dont_initialize();
  SC_REPORT_INFO( mesgType, ( "Constructed Stimulus_module \""s + name() + "\"" ).c_str() );
}

void Stimulus_module::stim_thread()
{
  Objection busy{ name() };
  wait( 100_ns );
  auto data = uint8_t{ static_cast<uint8_t>(Operation<>::Op::add) };
  auto delay = SC_ZERO_TIME;
  auto trans = tlm_generic_payload{};
  trans.set_command( tlm::TLM_WRITE_COMMAND );
  trans.set_address( 0 );
  trans.set_data_ptr( &data );
  trans.set_data_length( 1 );
  trans.set_streaming_width( trans.get_data_length() );
  trans.set_byte_enable_ptr( nullptr );
  trans.set_dmi_allowed( false );
  trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
  init_socket->b_transport( trans, delay );
  if( trans.is_response_error() ) {
    SC_REPORT_ERROR( mesgType, trans.get_response_string().c_str() );
  }
  wait( 10_ns );
}

#include <random>

void Stimulus_module::start_of_simulation()
{
  auto const testSize = 50;
  m_stimulus.reserve( testSize );
  auto gen = std::mt19937{};
  auto dist = std::uniform_int_distribution<int>(1,1000);
  for( auto& value : m_stimulus ) {
    value = dist(gen);
  }
}

void Stimulus_module::stim_method()
{
  if( clockin_port.read() ) { // posedge -- check received data
    if( datain_port->num_available() != 0 ) {
      auto data = datain_port->read();
      SC_REPORT_INFO_VERB( mesgType
                         , ( "Rcvd "s + std::to_string( data )
                           + " at " + sc_time_stamp().to_string() ).c_str()
                         , SC_DEBUG );
    }
  }
  else { // negedge -- send new data
    if( m_index < m_stimulus.size() ) {
      auto data = m_stimulus[ m_index++ ];
      dataout_port->put( data );
      SC_REPORT_INFO_VERB( mesgType
                         , ( "Sent "s + std::to_string( data )
                             + " at " + sc_time_stamp().to_string() ).c_str()
                         , SC_DEBUG );
    }
  }
}

//EOF

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
