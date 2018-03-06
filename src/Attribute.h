#ifndef LIBQCH5_ATTRIBUTE_H
#define LIBQCH5_ATTRIBUTE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"


namespace libqch5 {

/// An Attribute is a named tag that can be ascribed to another entity.
class Attribute {

   public:
      Attribute(String const& name, String const& value) : m_name(name), m_value(value) {  }

      String const& name() const { return m_name; }
      String const& value() const { return m_name; }

   protected:
      String m_name;
      String m_value;
};


class AttributeList : public List<Attribute> {

};

} // end namespace

#endif
