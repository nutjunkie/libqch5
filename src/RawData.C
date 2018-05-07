/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "RawData.h"
#include "H5Utils.h"
#include "hdf5_hl.h"


namespace libqch5 {

RawData::~RawData()
{
   List<ArrayBase*>::iterator iter;
   for (iter = m_arrays.begin(); iter != m_arrays.end(); ++iter) {
       DEBUG("Deleting array  " << *iter);
       delete *iter;
   }

   m_arrays.clear();
}


bool RawData::write(hid_t gid) const
{
   DEBUG("Writing RawData '" << m_label << "' of DataType '" << m_type << "' to " 
      << gid << " with " << m_arrays.size() << " arrays") ;

   hid_t wgid(openGroup(gid, m_label.c_str()));
   if (wgid < 0) return false;

   // DataType
   unsigned type(m_type.toUInt());
   H5LTset_attribute_uint(gid, m_label.c_str(), "DataType", &type, 1); 

   // Attributes
   m_attributes.write(gid, m_label.c_str());

   // Write array data
   int  index(0);
   bool ok(true);
   List<ArrayBase*>::const_iterator array;

   for (array = m_arrays.begin(); array != m_arrays.end(); ++array, ++index) {
       size_t const  rank((*array)->rank());
       size_t const* dimensions((*array)->dimensions());
       void   const* buffer((*array)->buffer());

       hid_t tid = (*array)->h5DataType();
       hsize_t* dims(new hsize_t[rank]);

       for (size_t i = 0; i < rank; ++i) {
           dims[i] = dimensions[i];
       }

       // The array data are named with an index
       String k(std::to_string(index));

       //DEBUG("Writing " << k << " to file, ptr-> " << *array << " type: " << tid);
       ok = ok && write(wgid, k.c_str(), tid, rank, dims, buffer);
       if (!ok)  DEBUG("WARN: Write failed for " << k);

       // This is how we could write attributes to specific arrays, if required:
       // const char* key("array_attr");
       // unsigned  value(1234567890);
       // herr_t status = H5LTset_attribute_uint(wgid, k.c_str(), key, &value, 1);
       // DEBUG("Setting array attribute " <<  key << " -> " << value << "  " << status);

       delete [] dims;
   }

   H5Gclose(wgid);

   return ok;
}


bool RawData::write(hid_t gid, char const* path, hid_t tid, size_t rank, 
   hsize_t const* dimensions, void const* data) const
{
   hid_t sid = H5Screate_simple(rank, dimensions, 0);
   hid_t did = H5Dcreate(gid, path, tid, sid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
       DEBUG("Data ID for " << path << " " << did);

   herr_t status = H5Dwrite(did, tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
   bool ok = (status == 0) &&  (H5Dclose(did) == 0) && (H5Sclose(sid) == 0);
          
   return ok;
}


bool RawData::read(hid_t gid)
{
   DEBUG("Reading data for " << m_label << " (" << gid << ")");
   bool ok(true);

   m_attributes.clear();
   m_attributes.read(gid, m_label.c_str());
   unsigned dataType(0);
   ok = m_attributes.get("DataType", dataType);
   DEBUG("DataType read as " << dataType);

   m_attributes.dump();

return true;

   hid_t wgid = H5Gopen(gid, m_label.c_str(), H5P_DEFAULT);
   if (wgid < 0) return false;

   herr_t status;

   // Read attributes
/*
   hid_t aid;
   unsigned n = H5Aget_num_attrs(gid);
   for (unsigned i = 0; i < n; ++i) {
       aid = H5Aopen_idx(gid, 
   }
*/
   


   //unsigned dt(999);;
   //status = H5LTget_attribute_uint(gid, m_label.c_str(), "attr_name", &dt);
   //DEBUG("Getting attribute " <<  m_label.c_str() << " attr_name " << dt << "  " << status);



   // Get number of data objects
   hsize_t count(0);
   herr_t err = H5Gget_num_objs(wgid, &count);
   if (err < 0) return false;
   DEBUG("Found " << count << " data objects in " << m_label);

   // And read them in
   for (hsize_t idx = 0; idx < count; ++idx) {
 
       // get length of name first..
       size_t len = H5Gget_objname_by_idx(wgid, idx, 0, 0);
       len += 1;  // ..add null termination..
       char* buff = new char[len]; 
       // ..and then read the name.  In most cases this will just be
       // the index.
       H5Gget_objname_by_idx(wgid, idx, buff, len);
       DEBUG("Reading dataset: " << buff);
	   int otype = H5Gget_objtype_by_idx(wgid, idx);

       switch (otype) {
          case H5G_GROUP:
             // Could enable recursive search for data.  We would need
             // to create a new RawData object here.
             DEBUG("WARN: Attempt to read subgroup in RawData::read");
             break;
          case H5G_DATASET:
             ok = ok && read(wgid, buff);
             break;
          case H5G_LINK:
          case H5G_TYPE:
          default:
             DEBUG("WARN: Unrecognised object type in RawData::read");
             ok = false;
             break;
       }

       delete [] buff;
   }

   H5Gclose(wgid);

   return ok;
}


bool RawData::read(hid_t gid, char const* path) 
{
   bool ok(true);

   hid_t did = H5Dopen(gid, path, H5P_DEFAULT);
   hid_t sid = H5Dget_space(did);
   hid_t tid = H5Dget_type(did);

   size_t rank(H5Sget_simple_extent_ndims(sid));

   hsize_t* dims(new hsize_t[rank]);
   hsize_t* max_dims(new hsize_t[rank]);

   H5Sget_simple_extent_dims(sid, dims, max_dims);

   for (unsigned i = 0; i < rank; ++i) {
       DEBUG("Reading array dimension: " << dims[i] << " of " << max_dims[i]);
   }

   // we are assuming double 
   ArrayBase* array(0);
   herr_t status(0);

   if (H5Tequal(tid, H5T_NATIVE_DOUBLE)) {
      switch (rank) {
         DEBUG("RawData::read reading H5T_NATIVE_DOUBLE");
         case 1:  array = &createArray<double>(dims[0]);                    break;
         case 2:  array = &createArray<double>(dims[0], dims[1]);           break;
         case 3:  array = &createArray<double>(dims[0], dims[1], dims[2]);  break;

         default: {
            DEBUG("Unsupported rank RawData::read " << rank);
            ok = false;
         } break;
      }

      void* buffer(array->buffer());
      status = H5Dread(did, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer);

   } else if (H5Tequal(tid, H5T_NATIVE_INT)) {
      switch (rank) {
         DEBUG("RawData::read reading H5T_NATIVE_INT");
         case 1:  array = &createArray<int>(dims[0]);                    break;
         case 2:  array = &createArray<int>(dims[0], dims[1]);           break;
         case 3:  array = &createArray<int>(dims[0], dims[1], dims[2]);  break;

         default: {
            DEBUG("Unsupported rank RawData::read " << rank);
            ok = false;
         } break;
      }

      void* buffer(array->buffer());
      status = H5Dread(did, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer);
      
   } else {
      DEBUG("Unknown data type in RawData::read  " << tid);
      DEBUG("Supported types:  H5T_NATIVE_INT    " << H5T_NATIVE_INT);
      DEBUG("Supported types:  H5T_NATIVE_DOUBLE " << H5T_NATIVE_DOUBLE);
      DEBUG("Supported types:  H5T_IEEE_F64LE    " << H5T_IEEE_F64LE);
      DEBUG("Supported types:  H5T_FLOAT         " << H5T_FLOAT);
      ok = false; 
 
   }

   ok = ok && status >= 0;

   delete [] dims;
   delete [] max_dims;

   H5Dclose(did);

   return ok;
}


} // end namespace
