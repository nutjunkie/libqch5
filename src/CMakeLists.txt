cmake_minimum_required(VERSION 3.1)

set(SRC
   Attributes.C
   DataType.C
   H5Utils.C
   Geometry.C
   ProjectFile.C
   Molecule.C
   RawData.C
   Schema.C
)

add_library( qch5 STATIC ${SRC})
