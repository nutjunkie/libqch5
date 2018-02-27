/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Data.h"
#include "Debug.h"

namespace libqch5 {

bool Data::append(String const& path, Data const& data)
{

}

template <class T>
bool Data::put(String const& path, T const& data)
{
   if (data )
   // split path
}

} // end namespace
