/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "RawData.h"


namespace libqch5 {

class Molecule : public RawData {

   public:
      Molecule(String const& name) : RawData(DataType::Molecule, name) { }

      void setAtoms(List<unsigned> const& atoms);

   private:
      unsigned m_nAtoms;
};

} // end namespace
