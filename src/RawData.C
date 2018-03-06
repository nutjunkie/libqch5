/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "RawData.h"


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


bool RawData::writeToFile(hid_t fid, char const* path) const
{
   List<ArrayBase*>::const_iterator iter;

   int i(0);
   for (iter = m_arrays.begin(); iter != m_arrays.end(); ++iter) {
       DEBUG("Writing " << path << " to file");

       size_t const  rank((*iter)->rank());
       size_t const* dimensions((*iter)->dimensions());
       void   const* buffer((*iter)->buffer());

       hid_t tid = (*iter)->h5DataType();
       hsize_t* dims(new hsize_t[rank]);

       for (size_t i = 0; i < rank; ++i) {
           dims[i] = dimensions[i];
       }

       write(fid, path, tid, rank, dims, buffer);
       delete [] dims;
   }

   return true;
}


bool RawData::write(hid_t fid, char const* path, hid_t tid, size_t rank, 
   hsize_t const* dimensions, void const* data) const
{
   hid_t sid = H5Screate_simple(rank, dimensions, 0);
   hid_t did = H5Dcreate(fid, path, tid, sid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   herr_t status;
   status = H5Dwrite(did, tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

   status = H5Dclose(did);
   status = H5Sclose(sid);
          
   return true;
}

} // end namespace
