#pragma once

#include <tlm_utils/multi_passthrough_target_socket.h>
#include <tlm_utils/multi_passthrough_initiator_socket.h>

struct Bus_module : sc_core::sc_module
{
  tlm_utils::multi_passthrough_target_socket<Bus_module>    targ_socket{ "targ_socket" };
  tlm_utils::multi_passthrough_initiator_socket<Bus_module> init_socket{ "init_socket" };
  explicit Bus_module( const sc_core::sc_module_name& nm );
  void b_transport( int id, tlm::tlm_generic_payload& trans, sc_core::sc_time& delay );
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
