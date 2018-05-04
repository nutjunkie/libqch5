/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "ProjectFile.h"
#include "H5Utils.h"
#include "RawData.h"

#include "hdf5_hl.h"

#include "Debug.h"

namespace libqch5 {

ProjectFile::ProjectFile(char const* path, Schema const& schema) : m_fileId(0), 
   m_status(Closed), m_schema(schema)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);
//H5G_loc_find
DEBUG("WARN: Don't truncate the file when opening");
   init(path);

   if (m_status != Open) {
      DEBUG("Failed to open project archive " << path);
   }else {
      DEBUG("Opened " << path);
   }
}


ProjectFile::ProjectFile(char const* path) : m_fileId(0), m_status(Closed)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);
   open(path);
}


ProjectFile::~ProjectFile()
{
   close();
}


void ProjectFile::open(char const* path)
{
   m_fileId = H5Fopen(path, H5F_ACC_RDWR, H5P_DEFAULT);

   if (m_fileId > 0) {
      DEBUG("Opening exisiting file " << path);
      m_status = Open;
   }else {
      DEBUG("Failed to open project archive " << path);
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
      m_error  = "Failed to create Schema group";
      m_status = Error;
   }
}


void ProjectFile::write(char const* path, RawData const& data)
{
   // Check if the data fits into the current Schema

   hid_t gid = openGroup(m_fileId, path);
   if (gid < 0) {
      DEBUG("!!! Failed to open group !!!" << path);
      return;
   }

   data.write(gid);
   H5Gclose(gid);
}


void ProjectFile::read(char const* path, RawData& data)
{

   // Check if the data fits into the current Schema
   hid_t gid = openGroup(m_fileId, path);
   if (gid < 0) {
      DEBUG("!!! Failed to open group !!!" << path);
      return;
   }

   data.read(gid);
   H5Gclose(gid);
}


bool ProjectFile::exists(char const* path) const
{
   hbool_t checkObjectValid(false);
   htri_t valid = H5LTpath_valid( m_fileId, path, checkObjectValid);
   //DEBUG("Path check returned: " << valid);
   return valid;
}


bool ProjectFile::exists(char const* path, RawData const& data) const
{
   // check path exists
   String p(path);
   p += "/" + data.label();
   hbool_t checkObjectValid(false);
   htri_t valid = H5LTpath_valid( m_fileId, p.c_str(), checkObjectValid);
   //DEBUG("Path check returned: " << valid);
   return valid;
}


bool ProjectFile::isValid(char const* path, RawData const& data) const
{
   bool valid(exists(path));
   valid = valid && m_schema.isValid(path, data);
   return valid;
}


bool ProjectFile::readSchema(Schema& schema)
{
   DEBUG("ProjectFile::readSchema NYI!!!");
   return true;
}


bool ProjectFile::writeSchema(Schema const& schema)
{
   DEBUG("ProjectFile::writeSchema NYI!!!");
   return true;
}

} // end namespace
