/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Molecule.h"
#include "Debug.h"

namespace libqch5 {

Molecule::Molecule(char const* name) : DataSpace(2,name);
{
//    addChunk(0); // nAtoms;
}

void Molecule::nAtoms(List<unsigned> const& atoms)
{
   addChunk
}

} // end namespace
