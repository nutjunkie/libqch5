/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "ProjectFile.h"
#include "Data.h"
#include "Debug.h"

namespace libqch5 {


ProjectFile::ProjectFile(char const* path) : m_fileId(0), m_status(Closed)
{
   open(path);
   if (m_status != Open)  init(path);

   if (m_status != Open) {
      DEBUG("Failed to open project archive " << path);
   }else {
      DEBUG("Opened " << path);
   }
}


ProjectFile::~ProjectFile()
{
   close();
}


void ProjectFile::open(char const* path)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);

   m_fileId = H5Fopen(path, H5F_ACC_RDWR, H5P_DEFAULT);

   if (m_fileId > 0) {
      DEBUG("Opening exisiting file " << path);
      m_status = Open;
   }
}


void ProjectFile::init(char const* path)
{
   m_fileId = H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
   
   if (m_fileId > 0) {
      DEBUG("Initializing new file " << path);
      initGroupHierarchy();
      m_status = Open;
   }
}


void ProjectFile::close()
{
   if (m_fileId > 0) H5Fclose(m_fileId);

   m_fileId = 0;
   m_status = Closed;
}


void ProjectFile::initGroupHierarchy()
{
   hid_t  group;
   herr_t status;

   group = H5Gcreate(m_fileId, "/Projects", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   if (group < 0) {
      m_error  = "Failed to create Projects group";
      m_status = Error;
   }

   group = H5Gcreate(m_fileId, "/External", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   if (group < 0) {
      m_error  = "Failed to create Exterals group";
      m_status = Error;
   }

   group = H5Gcreate(m_fileId, "/Consolidated", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   if (group < 0) {
      m_error  = "Failed to create Consolidated Data group";
      m_status = Error;
   }

   group = H5Gcreate(m_fileId, "/Schema", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   if (group < 0) {
      m_error  = "Failed to create Consolidated Data group";
      m_status = Error;
   }
}


void ProjectFile::put(char const* path, Data const& data)
{
   String p;
   if (path) p = String(path);
      
   unsigned level(data.level());
   for (unsigned i = 0; i < level; ++i) {
       p += "/default";
   }

   DEBUG("Adding data to path: " << p);
}


} // end namespace
