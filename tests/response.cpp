#include "response.h"
using namespace std::literals;
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

// Constructor
Response_module::Response_module( const sc_module_name& nm )
: sc_module( nm )
{
  datain_xport.bind( incoming_fifo );
  SC_HAS_PROCESS( Response_module );
  SC_METHOD( resp_method );
    sensitive << clockin_port;
    dont_initialize();
  SC_REPORT_INFO( mesgType, ( "Constructed Response_module \""s + name() + "\"" ).c_str() );
  targ_socket.register_b_transport( this, &Response_module::b_transport );
}

void Response_module::resp_method()
{
  if ( incoming_fifo.nb_can_get() and dataout_port->num_free() > 0 ) {
    auto data = incoming_fifo.get();
    data = m_op.transform( data, m_prev );
    dataout_port->write( data );
    m_prev = data;
  }
}

void Response_module::b_transport( tlm_generic_payload &trans, [[maybe_unused]]sc_time &delay )
{
  auto portWidth = targ_socket.get_bus_width()/8;
  auto address = trans.get_address();
  auto data_ptr = trans.get_data_ptr();
  auto data_len = trans.get_data_length();
  auto enable_ptr = trans.get_byte_enable_ptr();
  auto stream_len = trans.get_streaming_width();
  auto command = trans.get_command();
  if( address != 0u         // only one address, zero accepted
   or data_len == 0         // need at least 1 byte
   or stream_len < data_len // no streaming
   or data_len > portWidth  // allow up to port width but no more
   or enable_ptr != nullptr // no byte enables
   ) {
    trans.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
    return;
  }
  if( command == tlm::TLM_WRITE_COMMAND ) {
    m_op.set( data_ptr[0] );
  } else if ( command == tlm::TLM_READ_COMMAND ){
    data_ptr[0] = m_op.get();
  }
  trans.set_response_status( TLM_OK_RESPONSE );
}

//EOF

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
