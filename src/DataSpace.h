#ifndef LIBQCH5_DATASPACE_H
#define LIBQCH5_DATASPACE_H
/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"

namespace libqch5 {

typedef unsigned Level;

/// Low-level base class for managing space for storing data
class DataSpace {

   public:
      Data(Level const level, String const& path) : m_level(level), m_path(path) { }

      Level level() const { return m_level; }
      String const& path() const { return m_path; }

   protected:
      void createChunk(unsigned n);

   private:

      class Chunk {
         public:
            Chunk(hid_t type) : m_type(type), m_rank(0), m_dimensions(0) 
            {
               m_hid = H5Screate(H5S_SCALAR);
            }

            Chunk(hid_t type, int rank, int const* dimensions) : m_type(type), m_rank(rank) 
            {
               m_dimensions = new int[rank];
               for (int i = 0; i < rank; ++i) {
                   m_dimensions[i] = dimensions[i];
               }
               m_hid = H5Screate_simple(rank, dimensions, 0);
            }

            ~Chunk() {
               if (m_dimensions) delete m_dimensions;
               if (m_hid > 0) H5Sclose(m_hid);
            }

         private:
            hid_t  m_hid;
            hid_t  m_type;
            int    m_rank;
            int*   m_dimensions;
      };

      List<Chunk*> m_types;
      String m_path;
      Level  m_level;

      //List<Attribute> m_attributes;
};

} // end namespace

#endif
