#ifndef LIBQCH5_DEBUG_H
#define LIBQCH5_DEBUG_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include <iostream>


#define DEBUG_SHORT(x) do { std::cerr << x << std::endl; } while (0)

#define DEBUG_LONG(x) do { std::cerr << __FILE__ << ":" << __LINE__ << " " \
   << x << std::endl; } while (0)

#define DEBUG DEBUG_SHORT

#endif
