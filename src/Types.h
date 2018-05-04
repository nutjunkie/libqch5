#ifndef LIBQCH5_TYPES_H
#define LIBQCH5_TYPES_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include <string>
#include <vector>
#include <map>

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
 #include <cxxabi.h>
#endif
#include <memory>
#include <cstdlib>


namespace libqch5 {

typedef std::string  String;

template<typename T>
struct List : public std::vector<T>  { };

template<typename T>
struct StringMap : public std::map<String, T>  { };


template <class T>
String type_name()
{
   typedef typename std::remove_reference<T>::type TR;
   std::unique_ptr<char, void(*)(void*)> own(
#ifndef _MSC_VER
       abi::__cxa_demangle(typeid(TR).name(), nullptr,nullptr, nullptr),
#else
       nullptr,
#endif
       std::free
    );

    String r = own != nullptr ? own.get() : typeid(TR).name();

    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

//#include "quaternion.h"
//typedef boost::qvm::quatc<double>  Quaternion;

} // end namespace

#endif
