/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "RawData.h"


namespace libqch5 {

class Geometry : public RawData {

   public:
      enum Units { Bohr = 0, Angstroms };

      Geometry(String const& name = String());

      void setAtoms(List<unsigned> atoms);
      void setCoordinates(List<double> xyz);

      Array<2> const& getCoordinates() const;

      void setUnits();

   private:
      unsigned m_nAtoms;
      Units m_units;
};

} // end namespace
