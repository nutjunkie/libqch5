#ifndef LIBQCH5_DATA_H
#define LIBQCH5_DATA_H

/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"
#include "Attribute.h"

namespace libqch5 {

/// Abstract base class for all nodes on the data hierarchy.
class Data {

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

      template <class T>
      bool put(String const& path, T const&);

      bool get(String const& path, Data &);

   private:
      ProjectFile* m_projectFile;
      String       m_ownPath;
      Level        m_level;

      //List<Attribute> m_attributes;
};

} // end namespace

#endif
