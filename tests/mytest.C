#include "ProjectFile.h"
#include "Molecule.h"


#include "Array.h"
#include "RawData.h"
#include "Schema.h"

#include <iostream>

using namespace libqch5;

int main()
{
   DEBUG("\n === Array ===");
   // Each Array<T> has associated Size and Index types that can be used for
   // setting up the size of the array and accessing its elements.
   Array<3>::Size dims = { 3, 4, 8 }; 
   Array<3> array(dims);

   // Fill array with offset data for debugging
   array.fill();  

   // We can access an element using an Index type:
   Array<3>::Index idx = { 0, 1, 3 }; 

   for (idx[0] = 0; idx[0] < array.dim(0); ++idx[0]) {
       DEBUG("Array element ("<< idx[0] << "," << idx[1] << ","
               << idx[2] << ") = " << array(idx));
   }

   /// We can also access an element directly using aggregate initialization:
   array({3,4,2}) = 345;

   /// The default type of an array is double, but the class is templated:
   Array<1>::Size d = { 8 }; 
   Array<1, int> list(d);


   DEBUG("\n === RawData ===");
   // The RawData class has convenience functions that create array objects
   // and manage their data.  These are only available for D1-3
   RawData data("Ethanol", DataType::Molecule);
   Array<1>& d1(data.createArray(10));
   Array<2>& d2(data.createArray(4,6));
   Array<3>& d3(data.createArray(6,5,4));

   // The arrays can be accessed via their handles.
   d1.fill();  d2.fill();  d3.fill();
   d1.dump();  d2.dump();  d3.dump();

   Array<1>::Index idx1 = {9};
   Array<2>::Index idx2 = {3,5};
   Array<3>::Index idx3 = {2,3,3};

   std::cout << "test print " << d1(idx1) << " length " << d1.length() << std::endl;
   std::cout << "test print " << d2(idx2) << " length " << d2.length() << std::endl;
   std::cout << "test print " << d3(idx3) << " length " << d3.length() << std::endl;

   /// Attributes can be ascribed to the RawData objects.  These are small data
   /// (double, int, string) that do not need memory managemant.
   data.setAttribute("T1", 50);
   data.setAttribute("convergence", 5);
   data.setAttribute("theory", "b3lyp");
   data.setAttribute("pi", 3.1415);
   data.setAttribute("T1", 3);
   

   DEBUG("\n === Schema ===");
   // Schema form a tree structure that determines what data can reside where 
   Schema schema;
   Schema::Node& root(schema.root());
      Schema::Node& projects(root.appendChild(DataType::ProjectGroup));

      Schema::Node& molecules(projects.appendChild(DataType::MoleculeGroup));

      Schema::Node& molecule(root.appendChild(DataType::Molecule));
         Schema::Node& calculations(molecule.appendChild(DataType::Calculation));
         Schema::Node& geometry(molecule.appendChild(DataType::Geometry));
         Schema::Node& state(geometry.appendChild(DataType::State));
      Schema::Node& thermochemical(root.appendChild(DataType::Property));
      Schema::Node& zpve(thermochemical.appendChild(DataType::Property));

   schema.print();

   // These might appear to be off-by-one isValid -> canAppend?
   bool valid;
   valid = schema.isValid("/projects", DataType::Base);
   valid = schema.isValid("/projects", DataType::State);
   valid = schema.isValid("/projects/molecule/geom", DataType::Geometry);

   
   DEBUG("\n === ProjectFile ===");
   // ProjectFiles are a file with a given Schema
   libqch5::ProjectFile project("myproject.h5", ProjectFile::New, schema);

   project.write("/Projects", data);
   DEBUG("\n======================================================\n");

   DEBUG("Check this: " << project.pathExists("/Projects"));
   DEBUG("Check this: " << project.pathExists("/Projects/Untitled"));
   DEBUG("Check this: " << project.pathExists("/Projects/Ethanol/0"));
   DEBUG("Check this: " << project.isValid("/Projects", data));

   DEBUG("\n======================================================\n");

   RawData data2("Ethanol/", DataType::Molecule);
   project.read("/Projects/Ethanol", data2);

   project.write("/Projects2", data2);

   schema.isValid("/Projects", data.dataType());
   schema.isValid("/Projects/Molecules/", data.dataType());

/*
   unsigned depth(schema.depth(DataType::Molecule));
   DEBUG("Ply depth " << depth);
   DEBUG("");
   DEBUG("");
*/
   
   return 0;
}
