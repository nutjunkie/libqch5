cmake_minimum_required(VERSION 3.1)

set(SRC
   Attributes.C
   DataType.C
   ProjectFile.C
   RawData.C
   Schema.C
)

add_library( qch5 STATIC ${SRC})
