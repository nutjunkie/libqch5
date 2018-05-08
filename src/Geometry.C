/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Geometry.h"
#include "Debug.h"

namespace libqch5 {

Geometry::Geometry(char const* name) : RawData(name, DataType::Geometry)
{
   setAttribute("units", Bohr);
}


} // end namespace
