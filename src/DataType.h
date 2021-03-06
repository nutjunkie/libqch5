#ifndef LIBQCH5_DATATYPE_H
#define LIBQCH5_DATATYPE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Types.h"


namespace libqch5 {

class DataType {

   public:
	  /// Note the default id is Base allowing user defined data strucutures as
	  /// long as they interface generically.
      enum Id { Base = 0,
                Group, 
                Project, 
                Molecule,
                Geometry,
                State,
                Orbitals,
                Calculation,
                Property,
                Externals,
                BasisSet,
                  // Are these needed?
                  ProjectGroup,
                  MoleculeGroup,
                  GeometryGroup,
                  StateGroup,
                  CalculationGroup,
                  PropertyGroup,
                Invalid
              };

      DataType();
      DataType(String const&);
      DataType(unsigned const);
      DataType(DataType const& that);
    
      unsigned toUInt() const;
      String toString() const;
      static String toString(Id const);

      bool operator==(DataType const& rhs) const;
      bool operator!=(DataType const& rhs) const;

      friend std::ostream& operator<<(std::ostream& out, DataType const& data)
      {
         out << data.toString();
         return out;
      }

   private:
      Id m_id;
};


} // end namespace

#endif
