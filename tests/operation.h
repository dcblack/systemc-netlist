#pragma once
// Performs operations on data

#include <cstdint>
#include <string>
using namespace std::literals;

template<typename T=int>
class Operation {
public:
  enum class Op: uint8_t {
    nop, negate, add, flip, mask, merge, toggle, COUNT
  };
  const std::string names[static_cast<int>(Op::COUNT)]
  { "nop", "negate", "add", "mask", "merge", "toggle" };
  Operation() : Operation{ Op::nop } {}
  explicit Operation( Op op ) : m_op{op} {}
  void set( Op op )  { m_op = op; }
  void set( int op ) { m_op = static_cast<Op>( op ); }
  void set( const std::string& op ) {
    for( auto result{ Op::nop }; result != Op::COUNT; ++result ) {
      if( op == names[result]) return result;
    }
    return Op::nop;
  }
  [[nodiscard]]int get() const { return static_cast<int>( m_op ); }
  [[nodiscard]]explicit operator std::string() const {
    return names[ static_cast<int>( m_op ) ];
  }
  T transform( const T& lhs, const T& rhs )
  {
    switch( m_op ) {
      case Op::nop:      return lhs;
      case Op::negate:   return -lhs;
      case Op::add:      return lhs + rhs;
      case Op::flip:     return ~lhs;
      case Op::mask:     return lhs & rhs;
      case Op::merge:    return lhs | rhs;
      case Op::toggle:   return lhs ^ rhs;
      default:           throw std::runtime_error("Bad operation: "s + std::to_string( static_cast<int>(m_op) ) );
    }
  }
private:
  Op m_op{ Op::nop };
};
// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
