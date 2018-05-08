#ifndef LIBQCH5_H5UTILS_H
#define LIBQCH5_H5UTILS_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "hdf5.h"
#include "Types.h"


namespace libqch5 {

inline hid_t H5DataType(double) { return H5T_NATIVE_DOUBLE; }
inline hid_t H5DataType(int)    { return H5T_NATIVE_INT; }
inline hid_t H5DataType(String) { return H5T_STRING; }


/// Convenience function that attempts to open a group, and if that fails,
/// create it then open it.  Note the parent can be either a file ID, or
/// another group ID.
hid_t openGroup(hid_t parent, char const* group);

/// List the contents of the given location handle.
void listGroup(hid_t const gid);

/// Gets the length of the string attribute, returning zero if the
/// attribute does not exist.
hsize_t stringAttributeSize(hid_t oid, const char* attributeName);

} // end namespace

#endif
