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

   DEBUG("Writing Integer Attributes:");
   StringMap<int>::const_iterator iIter;
   for (iIter = m_attributesInt.begin(); iIter != m_attributesInt.end(); ++iIter) {
       const char* key(iIter->first.c_str());
       int value(iIter->second);
       status = H5LTset_attribute_int(oid, label, key, &value, 1); 
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   
   DEBUG("");

   DEBUG("Writing Double Attributes:");
   StringMap<double>::const_iterator dIter;
   for (dIter = m_attributesDouble.begin(); dIter != m_attributesDouble.end(); ++dIter) {
       const char* key(dIter->first.c_str());
       double value(dIter->second);
       status = H5LTset_attribute_double(oid, label, key, &value, 1); 
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   
   DEBUG("");

   DEBUG("Writing String Attributes:");
   StringMap<String>::const_iterator sIter;
   for (sIter = m_attributesString.begin(); sIter != m_attributesString.end(); ++sIter) {
       const char* key(sIter->first.c_str());
       const char* value(sIter->second.c_str());
       status = H5LTset_attribute_string(oid, label, key, value);
       ok = ok && (status == 0); 
       DEBUG("Setting attribute for " <<  label << ": " << key << " ->  " << value
             << "  (status = " << status << ")");
   }   
   DEBUG("");

   return ok;
}


bool Attributes::read(hid_t oid, char const* label) 
{
   bool ok(true);

   DEBUG("Reading attribute for " << oid);

   hid_t aid;
   hid_t atype;
   unsigned n(H5Aget_num_attrs(oid));
   DEBUG("Found " << n << " attributes");
   for (unsigned i = 0; i < n; ++i) {

       aid = H5Aopen_idx(oid, i);

       char* buffer;
       size_t length(H5Aget_name(aid, 0, buffer));
       buffer = new char[length+1];
       H5Aget_name(aid, length+1, buffer);
       DEBUG("Found attribute: " << aid << " name - " << buffer);
       atype = H5Aget_type(aid);
       H5Tclose(atype);

    H5T_class_t t_class = H5Tget_class(aid);

  if(t_class < 0){ 
        puts(" Invalid datatype.\n");
    } else {
        /*  
         * Each class has specific properties that can be 
         * retrieved, e.g., size, byte order, exponent, etc. 
         */
        if(t_class == H5T_INTEGER) {
              DEBUG(" Datatype is 'H5T_INTEGER'.\n");
            /* display size, signed, endianess, etc. */
        } else if(t_class == H5T_FLOAT) {
              DEBUG(" Datatype is 'H5T_FLOAT'.\n");
            /* display size, endianess, exponennt, etc. */
        } else if(t_class == H5T_STRING) {
              DEBUG(" Datatype is 'H5T_STRING'.\n");
            /* display size, padding, termination, etc. */
        } else if(t_class == H5T_BITFIELD) {
              DEBUG(" Datatype is 'H5T_BITFIELD'.\n");
            /* display size, label, etc. */
        } else if(t_class == H5T_OPAQUE) {
              DEBUG(" Datatype is 'H5T_OPAQUE'.\n");
            /* display size, etc. */
        } else if(t_class == H5T_COMPOUND) {
              DEBUG(" Datatype is 'H5T_COMPOUND'.\n");
            /* recursively display each member: field name, type  */
        } else if(t_class == H5T_ARRAY) {
              DEBUG(" Datatype is 'H5T_COMPOUND'.\n");
            /* display  dimensions, base type  */
        } else if(t_class == H5T_ENUM) {
              DEBUG(" Datatype is 'H5T_ENUM'.\n");
            /* display elements: name, value   */
        } else  {
              DEBUG(" Datatype is 'Other'.\n");
              /* eg. Object Reference, ...and so on ... */
        }   
    } 



       H5Aclose(aid);
   }

   return ok;
}


  
void Attributes::clear()
{
  m_attributesInt.clear();
  m_attributesDouble.clear();
  m_attributesString.clear();
}

} // end namespace
