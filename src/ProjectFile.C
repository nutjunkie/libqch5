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

ProjectFile::ProjectFile(char const* path, IOStat const ioStat, Schema const& schema) :
   m_fileId(0), m_status(Closed), m_schema(schema)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);

   open(path, ioStat);

   if (m_status == Open) {
       DEBUG("Opened project file: " << path);
       m_status = Open;
       if (ioStat == New || ioStat == Overwrite) {
          if (!writeSchema(m_schema)) DEBUG("ERROR: " << m_error);
       }

       Schema sch;       // !!!!!!!!!!!!!!!!!!
       readSchema(sch) ; // !!!!!!!!!!!!!!!!!!
    }else {
       DEBUG("Failed to open project file " << path);
    }
}


ProjectFile::~ProjectFile()
{
   close();
}


void ProjectFile::open(char const* path, IOStat const ioStat)
{
   switch (ioStat) {

      case New:
         DEBUG("Attempt to open new project archive: " << path);
         m_fileId = H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
         break;

      case Old:
         DEBUG("Attempt to open existing project archive: " << path);
         DEBUG("WARN: Need to check if file exists before opening");
         m_fileId = H5Fopen(path, H5F_ACC_RDWR, H5P_DEFAULT);
         break;

      case Overwrite:
         DEBUG("Opening project archive: " << path);
         m_fileId = H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
         break;
   }

   if (m_fileId > 0) m_status = Open;
}


void ProjectFile::close()
{
   if (m_fileId > 0) H5Fclose(m_fileId);
   m_fileId = 0;
   m_status = Closed;
}


void ProjectFile::write(char const* path, RawData const& data)
{
   if (isValid(path, data)) {
      hid_t gid = openGroup(m_fileId, path);
      data.write(gid);
      H5Gclose(gid);
   }else {
      DEBUG("WARN: Attempt to write invalid data for current schema.");
      DEBUG(data.dataType().toString() << " written to " << path);
   }
}


void ProjectFile::read(char const* path, RawData& data)
{
   // TODO: Check if the data fits into the current Schema
   DataType const& type(data.dataType());
   if (type == DataType::Base) {
      // new target ripe for filling.
      DEBUG("Reading into uninitialised data object");
   }else if (isValid(path, data)) {
      // existing target good to go
      DEBUG("Reading into existing data object of type " << data.dataType().toString());
   }else {
      // throw an error
      DEBUG("Incompatible data types, what are you doing?");
      return;
   }

   hid_t gid = H5Gopen(m_fileId, path, H5P_DEFAULT);
   if (gid < 0) {
      DEBUG("!!! Failed to open group !!!" << path);
      return;
   }

   // Grab the name from the /file/path/name passed in
   String label(path);
   size_t n(label.find_last_of('/'));

   data.setLabel(label.substr(n+1));
   data.read(gid);

   H5Gclose(gid);
}


bool ProjectFile::pathExists(char const* path) const
{
   hbool_t checkObjectValid(false);
   return H5LTpath_valid(m_fileId, path, checkObjectValid);
}


/*
bool ProjectFile::exists(char const* path, RawData const& data) const
{
   // check path exists
   String p(path);
   p += "/" + data.label();
   hbool_t checkObjectValid(false);
   htri_t valid = H5LTpath_valid( m_fileId, p.c_str(), checkObjectValid);
   return valid;
}
*/


bool ProjectFile::isValid(char const* path, RawData const& data) const
{
   bool valid(pathExists(path));
   valid = valid && m_schema.isValid(path, data.dataType());
   return valid;
}



bool ProjectFile::addGroup(char const* path)
{
   bool ok(true);

   if (!pathExists(path)) {
      hid_t group = H5Gcreate(m_fileId, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      if (group < 0) {
         m_error  = "Failed to add group ProjectFile: ";
         m_error += path;
         ok = false;
      }
   }

   return ok;
}


bool ProjectFile::readSchema(Schema& schema)
{
   hsize_t dims[6];
   H5T_class_t type;
   size_t size[6];
   H5LTget_attribute_info( m_fileId, "/", "Schema",  dims, &type, size );
DEBUG("H5LT info " << size[0]);

//   hsize_t size(stringAttributeSize(m_fileId, "Schema"));
/*
   hid_t aid = H5Aopen_name(m_fileId, "Schema");
   if (aid <= 0) return false;

   hid_t sid = H5Aget_space(aid);
   if (sid <= 0) return false;


   char* buffer;
   herr_t status(H5LTset_attribute_string(m_fileId, "/", "Schema", s.c_str()));

   bool ok(true);
   String s(m_schema.serialize());
   DEBUG("Deserializing Schema:\n"<<  s);
   m_schema.deserialize(s);

//   readSchema();
   herr_t ret = H5Aread(aid, tid, &value);
*/

   return true;
}


bool ProjectFile::writeSchema(Schema const& schema)
{
   String s(schema.serialize());
   herr_t status(H5LTset_attribute_string(m_fileId, "/", "Schema", s.c_str()));

   if (status != 0) {
      m_error  = "Failed to open group for Schema";
      m_status = Error;
   }

   return status == 0;;
}

} // end namespace
