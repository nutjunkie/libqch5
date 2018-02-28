/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "DataSpace.h"

namespace libqch5 {

class Molecule : public DataSpace {

   public:
      Molecule() : Data(Data::Molecule) { }

   private:
      unsigned m_nAtoms;
};

} // end namespace


