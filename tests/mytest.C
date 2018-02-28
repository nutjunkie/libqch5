#include "ProjectFile.h"
#include "Molecule.h"
#include "Geometry.h"

#include <iostream>


int main()
{
   hid_t   file_id;
   herr_t  status;

/*
   file_id = H5Fcreate("myfile.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

   int data[4][6] = { 1, 2, 3, 4,
                      5, 6, 7, 8,
                      9,10,11,12,
                     13,14,15,16,
                     17,18,19,20,
                     21,22,23,24 };

   hsize_t dims[2] = { 4, 6};
   hid_t   dataspace_id = H5Screate_simple(2, dims, NULL);


   hid_t  group_id   = H5Gcreate(file_id, "MyGroup", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   status = H5Gclose(group_id);

   hid_t  group_id2  = H5Gcreate(file_id, "MyGroup2", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   hid_t  dataset_id = H5Dcreate(file_id, "/MyGroup2/dset", H5T_STD_I32BE, dataspace_id, 
                                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
   status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
   status = H5Gclose(group_id2);


   status = H5Dclose(dataset_id);
   status = H5Sclose(dataspace_id);
   status = H5Fclose(file_id);

*/

   libqch5::ProjectFile project("myproject.h5");

   if (project.status() == libqch5::ProjectFile::Open) {
      std::cout << "All's well" << std::endl;
   }

   libqch5::Molecule mol;
   libqch5::Geometry geom;

   project.put("example",mol);


   mol.put(geom);

   return 0;
}


   
