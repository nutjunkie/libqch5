#ifndef LIBQCH5_GEOMETRY_H
#define LIBQCH5_GEOMETRY_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Data.h"

namespace libqch5 {

class Geometry : public Data {

   public:
      Geometry() : Data(Data::Geometry) { }
};

} // end namespace

#endif
