#include "ProjectFile.h"
#include "Molecule.h"
#include "Geometry.h"
#include "DataSpace.h"


#include "Array.h"
#include "RawData.h"

#include <iostream>

using namespace libqch5;

int main()
{
   hid_t   file_id;
   herr_t  status;

   file_id = H5Fcreate("myfile.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

/*
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



   if (project.status() == libqch5::ProjectFile::Open) {
      std::cout << "All's well" << std::endl;
   }

   libqch5::Molecule mol;
   libqch5::Geometry geom;

   //libqch5::DataSpace dataSpace;

   project.put("example",mol);


   //mol.put(geom);

*/

   status = H5Fclose(file_id);


   Array<3>::Size dims = { 3, 4, 8 }; 

   Array<3> array(dims);
   array.clear();
   array.fill();

   Array<3>::Index idx = { 0, 1, 3 }; 

   for (idx[0] = 0; idx[0] < array.dim(0); ++idx[0]) {
   array(idx);
   }


   Array<1>::Size d = { 8 }; 
   Array<1>::Index i = { 3 }; 
   Array<1, int> list(d);
   list.fill();

   for (i[0] = 0; i[0] < list.dim(0); ++i[0]) {
       list(i);
   }


   RawData data;

   Array<1>& d1(data.createArray(10));
   Array<2>& d2(data.createArray(4,6));
   Array<3>& d3(data.createArray(6,5,4));

   Array<4>& d4(data.createArray<4,double>({2,3,4,5}));

   d1.fill();
   d2.fill();
   d3.fill();

   d1.dump();
   d2.dump();
   d3.dump();



   Array<1>::Index idx1 = {9};
   Array<2>::Index idx2 = {3,5};
   Array<3>::Index idx3 = {2,3,3};

   std::cout << "test print " << d1(idx1) << " length " << d1.length() << std::endl;
   std::cout << "test print " << d2(idx2) << " length " << d2.length() << std::endl;
   std::cout << "test print " << d3(idx3) << " length " << d3.length() << std::endl;
   
   for (size_t j = 0; j < d2.dim(1); ++j) {
       for (size_t i = 0; i < d2.dim(0); ++i) {
           std::cout << "Array<2> " << d2({i,j}) << std::endl;
       }
   }

   d3({3,4,2}) = 345;

#if 0
   for (idx3[2] = 0; idx3[2] < 4; ++idx3[2]) {
       for (idx3[1] = 0; idx3[1] < 5; ++idx3[1]) {
            for (idx3[0] = 0; idx3[0] < 6; ++idx3[0]) {
			    std::cout << "Array<3> " << idx3[0] << " "
                                         << idx3[1] << " " 
                                         << idx3[2] << "   " 
                                         << d3(idx3) << std::endl;
            }
       }
   }
#endif

   libqch5::ProjectFile project("myproject.h5");

   project.put("/data", data);

   
   return 0;
}
