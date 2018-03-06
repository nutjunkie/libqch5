cmake_minimum_required(VERSION 3.3)

set(SRC
   ProjectFile.C
   RawData.C
)

add_library( qch5 STATIC ${SRC})
