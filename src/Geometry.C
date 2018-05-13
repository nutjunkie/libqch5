/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Geometry.h"
#include "Debug.h"

namespace libqch5 {


Geometry::Geometry(String const& name ) : RawData(DataType::Geometry, name) 
{
   setAttribute("units", Bohr);
}


} // end namespace
