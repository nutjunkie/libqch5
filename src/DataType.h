#ifndef LIBQCH5_DATATYPE_H
#define LIBQCH5_DATATYPE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"


namespace libqch5 {

class DataType {

   public:
      enum Id { Base = 0,
                ProjectGroup,
                Project, 
                MoleculeGroup,
                Molecule,
                GeometryGroup,
                Geometry,
                StateGroup,
                State,
                CalculationGroup,
                Calculation,
                PropertyGroup,
                Property,
                Invalid
              };

      static Id toId(String const&);
      static String toString(Id);

};

} // end namespace


#endif
