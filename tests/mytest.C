#include "ProjectFile.h"
#include "Molecule.h"


#include "Array.h"
#include "RawData.h"
#include "Schema.h"

#include <iostream>

using namespace libqch5;

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



   if (project.status() == libqch5::ProjectFile::Open) {
      std::cout << "All's well" << std::endl;
   }

   libqch5::Molecule mol;
   libqch5::Geometry geom;

   //libqch5::DataSpace dataSpace;

   project.("example",mol);



   status = H5Fclose(file_id);
*/

   // Array
   
   /// Each Array<T> has associated Size and Index types that can be used for
   /// setting up the size of the array and accessing its elements.

   Array<3>::Size dims = { 3, 4, 8 }; 
   Array<3> array(dims);

   array.fill();  // Fill array with offset data for debugging

   /// We can access an element using an Index type:
   Array<3>::Index idx = { 0, 1, 3 }; 

   for (idx[0] = 0; idx[0] < array.dim(0); ++idx[0]) {
       DEBUG("Array element ("<< idx[0] << "," << idx[1] << ","
               << idx[2] << ") = " << array(idx));
   }

   /// We can also access an element directly using aggregate initialization:
   array({3,4,2}) = 345;

   /// The default type of an arry is double, but the class is templated:
   Array<1>::Size d = { 8 }; 
   Array<1, int> list(d);


   /// The RawData class has convenience functions that create array objects
   /// and manage their data.  These are only available for D1-3
   RawData data("Ethanol", DataType::Molecule);

   data.setAttribute("T1", 50);
   data.setAttribute("convergence", 23);
   data.setAttribute("Happiness", "very");
   data.setAttribute("pi", 3.1415);

   //data.setAttribute("T1", 3);

   Array<1>& d1(data.createArray(10));
   Array<2>& d2(data.createArray(4,6));
   Array<3>& d3(data.createArray(6,5,4));
   
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


   Schema schema;
   Schema::Node& root(schema.root());
      Schema::Node& molecules(root.appendChild(DataType::MoleculeGroup));
      Schema::Node& molecule(molecules.appendChild(DataType::Molecule));
         Schema::Node& calculations(molecule.appendChild(DataType::Calculation));
         Schema::Node& geometry(molecule.appendChild(DataType::Geometry));
      Schema::Node& thermochemical(root.appendChild(DataType::Property));

   std::cout << std::endl;
   schema.print();
   std::cout << std::endl;

   libqch5::ProjectFile project("myproject.h5", schema);

   project.write("/Projects", data);

   DEBUG("Check this: " << project.exists("/Projects"));
   DEBUG("Check this: " << project.exists("/Projects/Untitled"));
   DEBUG("Check this: " << project.exists("/Projects/Ethanol/0"));
   DEBUG("Check this: " << project.exists("/Projects", data));
   DEBUG("Check this: " << project.isValid("/Projects", data));

   DEBUG("\n======================================================\n");

   //RawData data2;
   RawData data2("Ethanol/", DataType::Molecule);
   project.read("/Projects/Ethanol", data2);
   return 0;

   project.write("/Projects2", data2);

   schema.isValid("/Projects", data);
   schema.isValid("/Projects/Molecules/", data);

   unsigned depth(schema.depth(DataType::Molecule));
   DEBUG("Ply depth " << depth);
   DEBUG("");
   DEBUG("");
   
   return 0;
}
