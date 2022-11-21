#include "netlist.h"

#include <tlm>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <typeinfo>
#ifndef NOBOOST
#include <boost/core/demangle.hpp>
#endif

namespace netList
{

  using namespace std::literals;
  using namespace sc_core;
  using std::string;

  namespace // anonymous
  {
    // Used by print_hierarchy() and object_info() to insert leader for depth
    std::string leader( ssize_t depth )
    {
      std::string result{};
      for ( auto level = depth - 1; level > 0; --level ) {
        #ifndef NOUTF8
        result += "│  ";
        #else
        result += "|  ";
        #endif
      }
      return result;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  sc_object* get_top( sc_object* obj_ptr ) {
    sc_assert( obj_ptr != nullptr );
    while( obj_ptr->get_parent_object() != nullptr ) {
      obj_ptr = obj_ptr->get_parent_object();
    }
    return obj_ptr;
  }

  //////////////////////////////////////////////////////////////////////////////
  std::string object_info( sc_object* obj_ptr, ssize_t depth, bool fullpath )
  {
    // IMPORTANT: This function has early returns where further hierarchy
    //            is not expected to be useful or overlaps may occur.
    // TODO: Zero in on specialization using RTTI..?

    sc_assert( obj_ptr != nullptr );

    auto result = ""s;
    //-- NAME & KIND -------------------------------------------------------------
    if( fullpath ) result += obj_ptr->name();
    else           result += obj_ptr->basename();

    auto kind = std::string( obj_ptr->kind() );
    result += ": "s + obj_ptr->kind();

    #ifndef NOBOOST
    if( sc_report_handler::get_verbosity_level() >= SC_DEBUG ) result += " ("s + boost::core::demangle(typeid(*obj_ptr).name()) + ")";
    #endif

    //-- PRIMITIVE TYPES ---------------------------------------------------------
    // Stop here for various basic types not handled deeply currently
    auto kinds = { "sc_clock", "sc_event_queue", "sc_mutex", "sc_semaphore", "sc_vector", "sc_vector_assembly" };
    if( std::find( kinds.begin(), kinds.end(), kind ) != kinds.end() ) return result;
    // The following remaining SystemC kinds are handled in some fashion:
    //   sc_buffer, sc_cthread_process, sc_export, sc_fifo, sc_fifo_in,
    //   sc_fifo_out, sc_in, sc_in_resolved, sc_in_rv, sc_inout, sc_inout_resolved,
    //   sc_inout_rv, sc_method_process, sc_module, sc_object, sc_out,
    //   sc_out_resolved, sc_out_rv, sc_port, sc_port_base, sc_prim_channel,
    //   sc_signal, sc_signal_channel, sc_signal_resolved, sc_signal_rv,
    //   sc_thread_process, tlm_analysis_port,

    //-- PROCESSES ---------------------------------------------------------------

    //TODO: Add more info about processes ???

    //-- ANALYSIS_PORTS ----------------------------------------------------------
    #ifdef SC_HAS_TLM_ANALYSIS_BASE
      auto aport_base_ptr = dynamic_cast<tlm::tlm_analysis_port_base*>( obj_ptr );
      if( aport_base_ptr != nullptr ) {
        auto nConnections = aport_base_ptr->size();
        if( nConnections == 0 ) {
          result += " DISCONNECTED";
        }
        else {
          for( auto i = 0 ; i < nConnections; ++i )
          {
            result += "\n"s + leader( depth + 1 );
            result += "  ["s + std::to_string(i) + "] -> ";
            auto intf = aport_base_ptr->get_base_interface(i);
            auto subscriber = dynamic_cast<sc_object*>(intf);
            if( subscriber != nullptr ) result += subscriber->name();
          }
        }
        return result;
      }
    #else
      #if defined(__clang__) || \
         (defined(__GNUC__) && ((__GNUC__ * 1000 + __GNUC_MINOR__) >= 4006))
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpedantic"
        #pragma GCC diagnostic ignored "-W#warnings"
        #warning "No SC_HAS_TLM_ANALYSIS_BASE"
        #pragma GCC diagnostic pop
      #endif
    #endif/*SC_HAS_TLM_ANALYSIS_BASE*/

    //-- TLM_SOCKETS -------------------------------------------------------------
    auto socket_ptr = dynamic_cast<tlm::tlm_base_socket_if*>( obj_ptr );
    if( socket_ptr != nullptr ) {
      //if( obj_ptr->kind() )
      auto tlm_port = &(socket_ptr->get_base_port());
      auto tlm_port_if = tlm_port->get_interface();
      auto tlm_port_chan = dynamic_cast<sc_object*>(tlm_port_if);
      if( tlm_port_chan != nullptr ) result += tlm_port_chan->name();
      return result;
    }

    //-- SC_EXPORTS -------------------------------------------------------------
    auto export_ptr = dynamic_cast<sc_export_base*>( obj_ptr );
    if( export_ptr != nullptr ) {
      result += " -> ";
      auto intf = export_ptr->get_interface();
      auto chan = dynamic_cast<sc_object*>(intf);
      if( chan != nullptr ) result += chan->name();
    }

    //-- SC_PORTS ----------------------------------------------------------------
    auto port_base_ptr = dynamic_cast<sc_port_base*>( obj_ptr );
    if( port_base_ptr != nullptr ) {
      auto nPorts = port_base_ptr->bind_count();
      switch( nPorts ) {
        case 0: {
          result += " UNCONNECTED";
          break;
        }
        case 1: {
          result += " -> ";
          auto intf = port_base_ptr->get_interface();
          auto chan = dynamic_cast<sc_object*>(intf);
          if( chan != nullptr ) result += chan->name();
          break;
        }
        //TODO: Display multiport connectivity
        default: {
          for( auto i = 0 ; i < nPorts; ++i )
          {
            result += "\n"s + leader( depth + 1 );
            result += "  ["s + std::to_string(i) + "] -> ";
            if( i == 0 ) {
              auto intf = port_base_ptr->get_interface();
              auto chan = dynamic_cast<sc_object*>(intf);
              if( chan != nullptr ) result += chan->name();
            } else {
              #ifdef SC_HAS_GET_BASE_INTERFACE
                auto intf = port_base_ptr->get_base_interface(i);
                auto chan = dynamic_cast<sc_object*>(intf);
                if( chan != nullptr ) result += chan->name();
              #else/*!SC_HAS_GET_BASE_INTERFACE*/
                #if defined(__clang__) || \
                   (defined(__GNUC__) && ((__GNUC__ * 1000 + __GNUC_MINOR__) >= 4006))
                  #pragma GCC diagnostic push
                  #pragma GCC diagnostic ignored "-Wpedantic"
                  #pragma GCC diagnostic ignored "-W#warnings"
                  #warning "No SC_HAS_GET_BASE_INTERFACE"
                  #pragma GCC diagnostic pop
                #endif
                result += "INDETERMINATE"s;
              #endif/*SC_HAS_GET_BASE_INTERFACE*/
            }
          }
          break;
        }
      }
    }

    //-- CHANNELS ----------------------------------------------------------------
    auto chan_ptr = dynamic_cast<sc_interface*>( obj_ptr );
    if( chan_ptr != nullptr ) {
      auto prim_ptr = dynamic_cast<sc_prim_channel *>( obj_ptr );
      auto modl_ptr = dynamic_cast<sc_module *>( obj_ptr );
      if      ( prim_ptr != nullptr ) result += " primitive";
      else if ( modl_ptr != nullptr ) result += " hierarchical";
      else                            result += " minimal";
      result += " channel";
      //auto proc_handle = sc_process_handle( obj_ptr );
      //TODO: Add attributes
    }

    return result;
  }

  //////////////////////////////////////////////////////////////////////////////
  void print_hierarchy( const std::vector<sc_object*>& children, ssize_t depth, std::ostream& os )
  {
    auto n = children.size();
    for( const auto& child : children ) {
      os << leader( depth );
      #ifndef NOUTF8
      os << ((n == 1) ? "└─ " : "├─ ");
      #else
      os << ((n == 1) ? "`- " : "|- ");
      #endif
      os << object_info( child, depth ) << '\n';
      auto grandchildren = child->get_child_objects();
      print_hierarchy( grandchildren, depth + 1 );
      --n;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  void print_from_top( sc_object* obj_ptr, std::ostream& os )
  {
    auto top = get_top( obj_ptr );
    os << "-- NETLIST ---------------------------------------------------------------------\n";
    os << object_info( top, 1 ) << '\n';
    auto children = top->get_child_objects();
    print_hierarchy( children, 1, os );
    os << "--------------------------------------------------------------------------------\n";
  }

  //////////////////////////////////////////////////////////////////////////////
  void print( std::ostream& os )
  {
    os << "-- NETLIST ---------------------------------------------------------------------\n";
    for( const auto top : sc_get_top_level_objects() ) {
      os << object_info( top, 1 ) << '\n';
      auto children = top->get_child_objects();
      print_hierarchy( children, 1, os );
      os << "\n";
    }
    os << "--------------------------------------------------------------------------------\n";
  }

  //////////////////////////////////////////////////////////////////////////////
  sc_object* process_top()
  {
    auto handle = sc_get_current_process_handle();
    auto obj_ptr = handle.get_process_object();
    auto top = get_top( obj_ptr );
    return top;
  }

  //////////////////////////////////////////////////////////////////////////////
  #ifdef TODO
  std::vector<sc_object*> search( [[maybe_unused]]const std::string& regex ) {
    std::vector<sc_object*> result{};
    for( const auto top : sc_get_top_level_objects() ) {
      //TODO: regex search
      // result.append( search_object( regex ) );
    }
    return result;
  }
  #endif/*TODO*/

  //////////////////////////////////////////////////////////////////////////////
}

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
