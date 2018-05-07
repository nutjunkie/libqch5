/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "hdf5_hl.h"
#include "Attributes.h"
#include "Debug.h"


namespace libqch5 {


bool Attributes::write(hid_t oid, char const* label) const
{
   bool ok(true);
   herr_t status;

   DEBUG("-- Writing Integer Attributes: --");
   StringMap<int>::const_iterator iIter;
   for (iIter = m_attributesInt.begin(); iIter != m_attributesInt.end(); ++iIter) {
       const char* key(iIter->first.c_str());
       int value(iIter->second);
       status = H5LTset_attribute_int(oid, label, key, &value, 1); 
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   

   DEBUG("-- Writing Unsigned Attributes: --");
   StringMap<unsigned>::const_iterator uIter;
   for (uIter = m_attributesUInt.begin(); uIter != m_attributesUInt.end(); ++uIter) {
       const char* key(uIter->first.c_str());
       unsigned value(uIter->second);
       status = H5LTset_attribute_uint(oid, label, key, &value, 1); 
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   

   DEBUG("-- Writing Double Attributes: --");
   StringMap<double>::const_iterator dIter;
   for (dIter = m_attributesDouble.begin(); dIter != m_attributesDouble.end(); ++dIter) {
       const char* key(dIter->first.c_str());
       double value(dIter->second);
       status = H5LTset_attribute_double(oid, label, key, &value, 1); 
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   

   DEBUG("-- Writing String Attributes: --");
   StringMap<String>::const_iterator sIter;
   for (sIter = m_attributesString.begin(); sIter != m_attributesString.end(); ++sIter) {
       const char* key(sIter->first.c_str());
       const char* value(sIter->second.c_str());
       status = H5LTset_attribute_string(oid, label, key, value);
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   
   return ok;
}


bool Attributes::read(hid_t oid, char const* label) 
{
   bool ok(true);

   unsigned n(H5Aget_num_attrs(oid));
   DEBUG("Found " << n << " attributes for " << oid);

   for (unsigned i = 0; i < n; ++i) {

       hid_t aid = H5Aopen_idx(oid, i);

       const size_t bufferSize(128);
	   char*  buffer = new char[bufferSize];
	   size_t length(H5Aget_name(aid, bufferSize-1, buffer));

       if (length+1 > bufferSize) {
          delete [] buffer;
          buffer = new char[length+1];
          H5Aget_name(aid, length+1, buffer);
       }

       hid_t tid = H5Aget_type(aid);
       if (H5Tequal(tid, H5T_NATIVE_INT)) {
          int value;
          herr_t ret = H5Aread(aid, tid, &value);
          DEBUG("Found int attribute: " << buffer << " => " << value);

       }else if (H5Tequal(tid, H5T_NATIVE_DOUBLE)) {
          double value;
          herr_t ret = H5Aread(aid, tid, &value);
          DEBUG("Found double attribute: " << buffer << " => " << value);

       }else if (H5Tequal(tid, H5T_NATIVE_UINT)) {
          // Currently the only unsgned attribute is the data type
          unsigned value;
          herr_t ret = H5Aread(aid, tid, &value);
          DEBUG("Found unsigned attribute: " << buffer << " => " << value);

       }else {
          DEBUG("Unrecognised attribute type: " << buffer << " (" << tid << ")");
       }

       delete [] buffer;

       H5Aclose(aid);
   }

   return ok;
}


  
void Attributes::clear()
{
  m_attributesInt.clear();
  m_attributesUInt.clear();
  m_attributesDouble.clear();
  m_attributesString.clear();
}

} // end namespace
