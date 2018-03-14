cmake_minimum_required(VERSION 3.1)

set(SRC
   DataType.C
   H5Utils.C
   ProjectFile.C
   RawData.C
   Schema.C
)

add_library( qch5 STATIC ${SRC})
