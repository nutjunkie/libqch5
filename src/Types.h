#ifndef LIBQCH5_TYPES_H
#define LIBQCH5_TYPES_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include <vector>
#include <string>
//#include "quaternion.h"

namespace libqch5 {

template<typename T>
struct List : public std::vector<T>  { };

typedef std::string                  String;
//typedef boost::qvm::quatc<double>  Quaternion;


} // end namespace

#endif
