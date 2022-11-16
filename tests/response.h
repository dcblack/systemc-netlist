#pragma once

#include "operation.h"
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

struct Response_module : sc_core::sc_module {
  static constexpr const char* mesgType = "/Doulos Inc/SystemC-Example/response";
  // Ports
  sc_core::sc_in<bool>                             clockin_port  { "clockin_port"     };
  sc_core::sc_export<tlm::tlm_fifo_put_if<int>>    datain_xport  { "datain_xport"     };
  sc_core::sc_port<sc_core::sc_fifo_out_if<int>>   dataout_port  { "dataout_port"     };
  tlm::tlm_fifo<int>                               incoming_fifo { "incoming_fifo", 2 };
  tlm_utils::simple_target_socket<Response_module> targ_socket   { "targ_socket"      };
  // Construction, setup & teardown
  explicit Response_module( const sc_core::sc_module_name& );
  ~Response_module() override = default;
private:
  // Attributes
  Operation<int> m_op{};
  int            m_prev{};
  // Processes
  void resp_method();
  // TLM methods
  void b_transport( tlm::tlm_generic_payload& trans, sc_core::sc_time& delay );
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
