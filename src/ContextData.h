#ifndef LIBQCH5_CONTEXTDATA_H
#define LIBQCH5_CONTEXTDATA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "RawData.h"
#include "DataType.h"


namespace libqch5 {

class ContextData {

   public:
      ContextData(DataType dataType) : m_dataType(dataType) { }


   private:
      DataType m_dataType;
      RawData  m_data;
      // Attributes
};

} // end namespace

#endif
