/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "ContextData.h"


namespace libqch5 {

class Molecule : public ContextData {

   public:
      Molecule(char const* name);

      void setAtoms(List<unsigned> atoms);

   private:
      unsigned m_nAtoms;
};

} // end namespace


