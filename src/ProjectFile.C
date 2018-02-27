/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "ProjectFile.h"
#include "Debug.h"

namespace libqch5 {


ProjectFile::ProjectFile(String const& path)
{
   open(path);
   if (m_status != Open) {
      init(path);
   }

   if (m_status != Open) {
      DEBUG("Failed to open file " << path);
   }
}


ProjectFile::~ProjectFile()
{
   close();
}

ProjectFile::close()
{
   if (m_fileId > 0) {
      H
   }
   m_fileId = 0;
   m_status = Closed
}


bool Data::append(String const& path, Data const& data)
{

}

template <class T>
bool Data::put(String const& path, T const& data)
{
   if (data )
   // split path
}

} // end namespace
