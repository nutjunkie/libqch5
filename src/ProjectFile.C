/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "ProjectFile.h"
#include "H5Utils.h"
#include "hdf5_hl.h"
#include "RawData.h"
#include "StringUtils.h"
#include <fstream>

#include "Debug.h"


namespace libqch5 {

ProjectFile::ProjectFile(char const* path, IOMode const ioMode, Schema const& schema) :
   m_fileId(0), m_ioStat(Closed), m_schema(schema), m_logLevel(Off)
{
   // Turn off automatic printing of error messages
   H5Eset_auto(0,0,0);
   open(path, ioMode, schema);
}


ProjectFile::~ProjectFile()
{
   close();
}


void ProjectFile::log(LogLevel level, String const& message) const
{
   if (level > m_logLevel) return;
   switch(level) {
      case Off:
         break;
      case Error:
         DEBUG("ERROR: " << message);
         break;
      case Warn:
         DEBUG("WARN: " << message);
         break;
      case Info:
         DEBUG("INFO: " << message);
         break;
   }
}


void ProjectFile::open(char const* path, IOMode const ioMode, Schema const& schema)
{
   if (m_ioStat == Open) {
      log(Warn, "Attempt to open existing ProjectFile: " + String(path));
      return;
   }

   if (ioMode == New || ioMode == Overwrite) {
      if (schema == Schema()) {
         m_error = "Empty Schema specified for ProjectFile: " + String(path);
         log(Error, m_error);
         return;
      }
   }

   switch (ioMode) {

      case New: {
         // Check for existance first so we don't overwrite
         std::ifstream f(path);
         if (f.good()) {
            m_error = "file already exists: " + String(path);
            log(Error, m_error);
            return;
         }else {
            m_fileId = H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
         } 
       } break;

      case Old:
         m_fileId = H5Fopen(path, H5F_ACC_RDWR, H5P_DEFAULT);
         break;

      case Overwrite:
         m_fileId = H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
         break;
   }

   if (m_fileId <= 0) {
      m_error = "Failed to open project file " + String(path);
      log(Error, m_error);
      return;
   }

   switch (ioMode) {

      case New:
      case Overwrite:
         if (writeSchema(schema)) {
            m_schema = schema;
            m_ioStat = Open;
         }else {
            m_error = "Failed to write schema to project file " + String(path);
            log(Error, m_error);
            close();
         }
         break;

      case Old:
         if (readSchema(m_schema) && (m_schema != Schema()) ) {
            if (schema == m_schema) {
               m_ioStat = Open;
            }else {
               m_error = "Mismatch in Schemata for ProjectFile: " + String(path);
               log(Error, m_error);
               m_schema.print();
               schema.print();
               close();
            }
         }else {
            m_error = "Failed to read valid schema from project file " + String(path);
            log(Error, m_error);
            close();
         }
         break;
   }
}


void ProjectFile::close()
{
   m_ioStat = Closed;
   if (m_fileId > 0) H5Fclose(m_fileId);
   m_fileId = 0;
}


bool ProjectFile::write(RawData const& data)
{
   // This should allow the file to be treated as a bucket, 
   // the path has to be determined based on the datatype
   return false;
}


bool ProjectFile::write(char const* path, RawData const& data)
{
   bool ok(false);

   if (pathCheck(path, data.dataType())) {

      hid_t gid = openGroup(m_fileId, path);
      if (gid > 0) {
         data.write(gid);
         H5Gclose(gid);
         DEBUG(data.dataType().toString() << " written to " << path << "/" << data.label());
         ok = true;
      }else {
         m_error = "Failed to open group " + String(path);
      }
   }else {
      m_error = "Failed to write " + data.dataType().toString()  + " to "
          + String(path) + " with current schema";
   }

   if (!ok) log(Error, m_error);
   
   return ok;
}


bool ProjectFile::pathCheck(char const* path, DataType const& dataType) const
{
   if (!pathExists(path)) return false;

   List<DataType> dataTypes(m_schema.find(dataType));
   std::vector<String> tokens(split(String(path),'/')); 

   // The path does not account for the actual data that is being written 
   // and so is 1 shorter.
   if (dataTypes.size() != tokens.size() +1) {
      log(Warn, "Path length does not match that for DataType");
      return false;
   }

   String p;
   for (size_t i = 0; i < tokens.size(); ++i) {
       p += "/" + tokens[i];
       if (getDataType(p.c_str()) != dataTypes[i]) {
          DEBUG("  pathCheck failed for " << p << " at level " << i);
          DEBUG("  " << getDataType(p.c_str()) << " != "  << dataTypes[i]);
          return false;
       }
   }

   return true;
}


bool ProjectFile::read(char const* path, RawData& data)
{
   if (!pathExists(path)) {
      m_error = "ProjectFile::read: Non-existent path " + String(path);
      log(Error, m_error);
      return false;
   }
   
   DataType dataType(getDataType(path));

   if (dataType != data.dataType()) {
      m_error = "ProjectFile::read: Incompatible data types - " + dataType.toString() + 
         " and " + data.dataType().toString();
      log(Error, m_error);
      return false;
   }

   hid_t gid = H5Gopen(m_fileId, path, H5P_DEFAULT);
   if (gid <= 0) {
      m_error = "ProjectFile::read: Failed to open path " + String(path);
      log(Error, m_error);
      return false;
   }

   // Grab the object name from the /file/path/name passed in
   String label(path);
   if (label.back() == '/') label.pop_back();  // strip any trailing '/'
   size_t n(label.find_last_of('/'));
   data.setLabel(label.substr(n+1));

   bool ok(data.read(gid));

   if (!ok) m_error = "ProjectFile::read: Data read failed for path " + String(path);
   if (ok) DEBUG("ProjectFile::read: " << dataType.toString() + " data read from " << path);

   H5Gclose(gid);
   if (!ok) log(Error, m_error);
   return ok;
}



bool ProjectFile::addGroup(char const* path, DataType const& dataType)
{
   if (m_ioStat != Open) return false;

   bool ok(false);

   if (pathExists(path)) {

      if (dataType == getDataType(path)) {
         ok = true;
      }else {
         m_error = "ProjectFile::addGroup: inconsistent group already exists for " 
            + String(path);
      }

   }else {

      // Strip off the group name for the path check
      String p(path);
      if (p.back() == '/') p.pop_back();  // strip any trailing '/'
      p = p.substr(0, p.find_last_of('/'));

      if (pathCheck(p.c_str(), dataType)) {
         hid_t gid = H5Gcreate(m_fileId, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

         if (gid > 0) {
            unsigned value(dataType.toUInt());
            herr_t herr = H5LTset_attribute_uint(gid, path, "DataType", &value, 1);
            if (herr == 0) {
               ok = true;
            }else {
               m_error = "ProjectFile::addGroup: Failed to set attribute for " + String(path);
            }
         }else {
            m_error = "ProjectFile::addGroup: Failed to add group: " + String(path);
         }
         if (gid > 0) H5Gclose(gid);

      } else {
         m_error = "Path check failed";
      }

   }

   if (!ok) log(Error, m_error);
   return ok;
}


bool ProjectFile::pathExists(char const* path) const
{
   if (m_ioStat != Open) return false;
   hbool_t checkObjectValid(false);
   return H5LTpath_valid(m_fileId, path, checkObjectValid);
}


DataType ProjectFile::getDataType(char const* path) const
{
   DataType invalid(DataType::Invalid);
   if (m_ioStat != Open) return invalid;

   if (!pathExists(path)) {
      DEBUG("ProjectFile::typeCheck: Non-existent path " << path);
      return invalid;
   }
   
   hid_t gid = H5Gopen(m_fileId, path, H5P_DEFAULT);
   if (gid <= 0) {
      DEBUG("ProjectFile::typeCheck: Failed to open path " << path);
      return invalid;
   }

   unsigned value;
   hid_t aid = H5Aopen_name(gid, "DataType");
   herr_t status = H5Aread(aid, H5T_NATIVE_UINT, &value);
   if (!(status == 0)) {
      DEBUG("ProjectFile::typeCheck: Failed to determine DataType for path " << path);
      H5Gclose(gid);
      return invalid;
   }

   H5Gclose(gid);
   return DataType(value);
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

   herr_t herr = H5LTget_attribute_string(m_fileId, "/", "Schema", buffer);

   bool ok(herr == 0);
   if (ok) {
      ok = schema.deserialize(buffer);
   }else {
      m_error  = "Failed to read Schema";
   }

   delete [] buffer;
   return ok;
}


bool ProjectFile::writeSchema(Schema const& schema)
{
   String s(schema.serialize());
   herr_t herr = H5LTset_attribute_string(m_fileId, "/", "Schema", s.c_str());

   return (herr == 0);
}

} // end namespace
