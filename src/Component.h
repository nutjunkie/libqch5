#ifndef LIBQCH5_COMPONENT_H
#define LIBQCH5_COMPONENT_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Types.h"


namespace libqch5 {

/// A component of a system that has a location and Attributes
class Component {

   public:
      Component() {  }

   protected:
      String          m_label;
      List<Attribute> m_attributes;
};

} // end namespace
