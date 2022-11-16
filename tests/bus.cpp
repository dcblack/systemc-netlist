#include "bus.hpp"

Bus_module::Bus_module( const sc_core::sc_module_name &nm )
: sc_module( nm )
{
  targ_socket.register_b_transport( this, &Bus_module::b_transport );
}
void Bus_module::b_transport( [[maybe_unused]]int id, tlm::tlm_generic_payload &trans, sc_core::sc_time &delay )
{
  init_socket[0]->b_transport( trans, delay );
}
// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
