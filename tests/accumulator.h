#pragma once

#include <systemc>

// Registered data clears and accumulates synchronous to the clock.
// add and clear inputs must be valid tSETUP before and held tHOLD after the positive clock edge.
// The result is valid asynchronously with the enable.
//        ____________________________________________         __________ 
// enable                                             \_______/           
//           :      :       :       :       :         :       :     :     
// result XXX< 0    X V   X V+I             X V+I+J   >???????< V+I+J     
//           :      :       :       :       :       :       :       :     
// datain --------< V >---< I >-----:-----< J         >------------------ 
//           :     ___     ___      :      ___      :       :       :     
//  add   XXXXXXXX/ : \___/ : \_____:_____/ : \_____:_______:_______:____ 
//          ___     :       :       :       :       :       :       :     
//  clear _/ : \____________________________________:_______:_______:____ 
//           :__    :___    :___    :___    :___    :___    :___    :___  
// clock  ___|  |___|   |___|   |___|   |___|   |___|   |___|   |___|   | 

template<size_t W>
SC_MODULE( Accumulator_module ) {
  using Data = sc_core::sc_signal<sc_dt::sc_int<W>>;
  // Ports
  sc_in<Data>  datain_port { "datain_port" };
  sc_in<bool>  add_port    { "add_port"    };
  sc_in<bool>  clear_port  { "clear_port"  };
  sc_out<Data> enable_port { "enable_port" };
  sc_out<Data> result_port { "result_port" };
  sc_in<bool>  clock_port  { "clock_port"  };
  // Constructor
  SC_CTOR( Accumulator_module ) {
    SC_METHOD( adding_method );
      sensitive << clock.pos();
    SC_METHOD( output_method );
      sensitive << enable_port;
  }
  // Processes
  void adding_method() {
    if( clear_port.read() ) {
      sum = 0;
    } else {
      sum += datain_port.read();
    }
  }
  void output_method() {
    if( enable_port.read() ) {
      result_port.write( sum );
    } else {
      result_port.write( 0xDEADBEEF );
    }
  }
  // Local attributes
  Data sum{};
};

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
