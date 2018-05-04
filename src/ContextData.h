#ifndef LIBQCH5_CONTEXTDATA_H
#define LIBQCH5_CONTEXTDATA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "RawData.h"


namespace libqch5 {


class ContextData {

   public:
      enum Type {
          Project
      };

      ContextData(Type type) : m_type(type)  { }

   private:
      Type     m_type;
      RawData  m_data;
      // Attributes
};

} // end namespace

#endif
