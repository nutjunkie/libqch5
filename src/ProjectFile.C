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

ProjectFile::ProjectFile(char const* path, IOMode const ioMode, Schema const& schema) :
   m_fileId(0), m_ioStat(Closed), m_schema(schema)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);
   open(path, ioMode, schema);
}


ProjectFile::~ProjectFile()
{
   close();
}


void ProjectFile::open(char const* path, IOMode const ioMode, Schema const& schema)
{
   if (m_ioStat == Open) {
      DEBUG("ERROR: Attempt to open existing ProjectFile" << path);
      return;
   }

   switch (ioMode) {

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

   if (m_fileId > 0) {

      m_ioStat = Open;
      DEBUG("Opened project file: " << path);
      m_ioStat = Open;
      if (ioMode == New || ioMode == Overwrite) {
         if (!writeSchema(m_schema)) DEBUG("ERROR: " << m_error);
      }else if (ioMode == Old) {
         readSchema(m_schema); 
         // make sure Schema match, if specified.
         if ((schema != Schema()) && (schema != m_schema)) {
            DEBUG("WARN: Mismatching schema specifed for ProjectFile:" << path);
            m_schema.print();
            schema.print();
         }
      }

   }else {
      DEBUG("Failed to open project file " << path);
   }
}


void ProjectFile::close()
{
   m_ioStat = Closed;
   if (m_fileId > 0) H5Fclose(m_fileId);
   m_fileId = 0;
}


void ProjectFile::write(char const* path, RawData const& data)
{
   if (isValid(path, data)) {
      hid_t gid = openGroup(m_fileId, path);
      data.write(gid);
      H5Gclose(gid);
      DEBUG(data.dataType().toString() << " written to " << path);
   }else {
      DEBUG("WARN: Cannot write " << data.dataType().toString() << " to " << path
          << " with current schema");
   }
}


bool ProjectFile::read(char const* path, RawData& data)
{
   // We read without doing any Schmea check as long as the RawData DataType
   // is consistent with the DataType of the path in the file.
   if (!pathExists(path)) {
      m_error = "ProjectFile::read: Non-existent path " + String(path);
      return false;
   }
   
   hid_t gid = H5Gopen(m_fileId, path, H5P_DEFAULT);
   if (gid <= 0) {
      m_error = "ProjectFile::read: Failed to open path " + String(path);
      return false;
   }

   // Determine the DataType of the file object
   unsigned value;
   hid_t aid = H5Aopen_name(gid, "DataType");
   herr_t status = H5Aread(aid, H5T_NATIVE_UINT, &value);
   if (!(status == 0)) {
      m_error = "ProjectFile::read: Failed to determine DataType for path " + String(path);
      H5Gclose(gid);
      return false;
   }

   DataType type(value);
   if (type != data.dataType()) {
      m_error = "ProjectFile::read: Incompatible data types - " + type.toString() + 
         " and " + data.dataType().toString();
      H5Gclose(gid);
      return false;
   }

   // Grab the object name from the /file/path/name passed in
   String label(path);
   if (label.back() == '/') label.pop_back();  // strip any trailing '/'
   size_t n(label.find_last_of('/'));
   data.setLabel(label.substr(n+1));

   bool ok(data.read(gid));

   if (!ok) m_error = "ProjectFile::read: Data read failed for path " + String(path);
   if (ok) DEBUG("ProjectFile::read: " << type.toString() + " data read from " <<path);

   H5Gclose(gid);
   return ok;
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
   
   //valid = valid && m_schema.type(path) == data.dataType();
   valid = valid && m_schema.isValid(path,  data.dataType());
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
   if (!H5LTfind_attribute(m_fileId, "Schema") ) return false;

   // Get length of the Schema string for buffer allocation
   hsize_t dims;
   H5T_class_t type;
   size_t length;
   H5LTget_attribute_info(m_fileId, "/", "Schema",  &dims, &type, &length);

   char* buffer(new char[length+1]);

   herr_t ioStat =  H5LTget_attribute_string(m_fileId, "/", "Schema", buffer);

   bool ok(ioStat == 0);
   if (ok) {
      ok = schema.deserialize(buffer);
      //schema.print();
   }else {
      m_error  = "Failed to read Schema";
   }

   delete [] buffer;
   return ok;
}


bool ProjectFile::writeSchema(Schema const& schema)
{
   String s(schema.serialize());
   herr_t ioStat(H5LTset_attribute_string(m_fileId, "/", "Schema", s.c_str()));

   if (ioStat != 0) {
      m_error  = "Failed to write Schema";
      m_ioStat = Error;
   }

   return ioStat == 0;;
}

} // end namespace
