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
   RawData data("Ethanol", DataType::State);
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
   
   RawData water("Water", DataType::Molecule);
   water.setAttribute("density", 1.2);

   RawData acetone("Acetone", DataType::Molecule);

   DEBUG("\n === Schema ===");
   // Schema form a tree structure that determines what data can reside where 
   Schema schema(DataType::Project);
   schema.root()
         .appendChild(DataType::Molecule)
         .appendChild(DataType::Geometry)
         .appendChild(DataType::State)
         .appendChild(DataType::Calculation)
         .appendChild(DataType::Property);

   schema.print();

   // Schema can also be build up
   Schema::Node& root(schema.root());
   Schema::Node& externals(root.appendChild(DataType::Group));
   Schema::Node& basis(externals.appendChild(DataType::BasisSet));


   
   DEBUG("\n === ProjectFile ===");
   // New ProjectFiles can be created with a given schema.
   // Note that this cannot be later changed.
   ProjectFile project("myproject.h5", ProjectFile::Overwrite, schema);

   if (!project.isOpen()) {
      DEBUG("Problem opening ProjectFile " << project.error());
      return 0;
   }

   // Existing ProjectFiles can be opened and, if specified, a schema
   // check can be made to ensure consistent schema.
   // Schema::Node& units(zpve.appendChild(DataType::Property));
   ProjectFile project2("old_project.h5", ProjectFile::Old, schema);

   DEBUG("\n======================================================\n");
   ProjectFile nonexist("something.h5", ProjectFile::Old, Schema());
   if (!nonexist.isOpen()) {
      DEBUG("Problem opening ProjectFile nonexist " << nonexist.error());
   }
   DEBUG("\n======================================================\n");

   project.addGroup("/Isomerization", DataType::Project);
   project.write("/Isomerization", water);
   project.write("/Isomerization", acetone);

return 0;

   // project.write("/", data);
   // project.write("/Junk", data);


/*
   DEBUG("\n======================================================\n");
   DEBUG("Check this: " << project.pathExists("/Projects"));
   DEBUG("Check this: " << project.pathExists("/Projects/Untitled"));
   DEBUG("Check this: " << project.pathExists("/Projects/Ethanol/0"));
   DEBUG("\n======================================================\n");
*/

   RawData data2("Ethanol/", DataType::Base);
   DEBUG("reading data into data2");

   if (!project.read("/Ethanol", data2)) {
      DEBUG("ERROR: " << project.error());
   }

   project.write("/Projects2", data2);

/*
   schema.isValid("/Projects", data.dataType());
   schema.isValid("/Projects/Molecules/", data.dataType());
*/

   return 0;
}
