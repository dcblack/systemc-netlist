#pragma once

#include <string>
using namespace std::literals;

namespace {

#if __cplusplus < 201103L
  #define CPP_VERSION 1998
#else
  #define CPP_VERSION ( __cplusplus / 10000L )
#endif

  inline std::string cppversion() {

    auto result = "Compiled with "s;
    result += "C++ std version: "s + std::to_string( __cplusplus ) + " (ISO/IEC 14882:"s + std::to_string( CPP_VERSION ) + ") "s;

    #if defined(__clang__)
      result += "Clang/LLVM version "s + std::to_string( __clang_major__ ) + "."s
              + std::to_string( __clang_minor__ ) + "."s + std::to_string( __clang_patchlevel__ );
    #elif defined(__ICC) || defined(__INTEL_COMPILER)
      result += "Intel ICC/ICPC version "s + std::to_string(__ICC );
    #elif defined(__GNUC__) || defined(__GNUG__)
      result += "GNU GCC/G++ version "s + std::to_string(__GNUC__ ) + "."s
              + std::to_string(__GNUC_MINOR__ ) + "."s + std::to_string(__GNUC_PATCHLEVEL__ );
    #elif defined(__HP_cc) || defined(__HP_aCC)
      result += "Hewlett-Packard C/aC++ version "s + std::to_string(__HP_cc );
    #elif defined(__IBMC__) || defined(__IBMCPP__)
      result += "IBM XL C/C++ version "s + std::to_string(__IBMCPP__ );
    #elif defined(_MSC_VER)
      result += "Microsoft Visual Studio version "s + std::to_string(_MSC_FULL_VER );
    #elif defined(__PGI)
      result += "Portland Group PGCC/PGCPP version "s + std::to_string(__PGIC__ )
              + "."s + std::to_string(__PGIC_MINOR__ ) + "."s
              + std::to_string(__PGIC_PATCHLEVEL__ );
    #elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
      result += "Oracle Solaris Studio version "s + std::hex
              + std::to_string(__SUNPRO_CC );
    #else
      result += "unknown"s;
    #endif

    return result;
  }
}

// ****************************************************************************
// This file is licensed under Apache-2.0, and
// Copyright 2022 David C Black <david.black@doulos.com>
// See accompanying LICENSE from https://www.apache.org/licenses/LICENSE-2.0.txt
