#ifndef LIBQCH5_SCHEMA_H
#define LIBQCH5_SCHEMA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"
#include "Attribute.h"

namespace libqch5 {

/// Schemas determine the heirarchy of the data storage
class Schema {

   public:
      enum Level { ProjectGroup = 0, 
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
                   Property 
                 };


      Data(Level const level) : m_level(level) { }

      Level level() const { return m_level; }

      bool append(String const& path, Data const&);

      bool put(Data const&);

      bool get(String const& path, Data &);

   private:
      //ProjectFile* m_projectFile;
      String       m_ownPath;
      Level        m_level;

      //List<Attribute> m_attributes;
};

} // end namespace


#endif


group
data_type  size  data