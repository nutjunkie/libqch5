#ifndef LIBQCH5_H5UTILS_H
#define LIBQCH5_H5UTILS_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "hdf5.h"


namespace libqch5 {

inline hid_t H5DataType(double) { return H5T_NATIVE_DOUBLE; }
inline hid_t H5DataType(int)    { return H5T_NATIVE_INT; }


/// Convenience function that attempts to open a group, and if that fails,
/// create it then open it.  Note the parent can be either a file ID, or
/// another group ID.
hid_t openGroup(hid_t parent, char const* group);


/// List the contents of the given location handle.
void listGroup(hid_t const gid);

} // end namespace

#endif
