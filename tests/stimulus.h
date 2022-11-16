#pragma once

#include <tlm_utils/simple_initiator_socket.h>

struct Stimulus_module : sc_core::sc_module {
  static constexpr const char* mesgType = "/Doulos Inc/SystemC-Example/stimulus";
  // Ports
  sc_core::sc_in<bool>                                clockin_port { "clockin_port" };
  sc_core::sc_port<tlm::tlm_fifo_put_if<int>>         dataout_port { "dataout_port" };
  sc_core::sc_port<sc_core::sc_fifo_in_if<int>>       datain_port  { "datain_port"  };
  tlm_utils::simple_initiator_socket<Stimulus_module> init_socket  { "init_socket"  };

  // Construction, setup & teardown
  explicit Stimulus_module( const sc_core::sc_module_name& );
  void start_of_simulation() override;
  ~Stimulus_module() override = default;
private:
  // Processes
  void stim_thread();
  void stim_method();
  // Attributes
  std::vector<int> m_stimulus;
  size_t           m_index{};
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
