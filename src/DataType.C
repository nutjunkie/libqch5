/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "DataType.h"


namespace libqch5 {

bool caseInsensitiveEquals(const String& a, const String& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}


DataType::DataType() : m_id(Base)
{ 
}


DataType::DataType(DataType const& that)
{ 
   m_id = that.m_id; 
}


DataType::DataType(String const& s) : m_id(Base)
{
   for (unsigned i = 0; i < Invalid; ++i) {
       Id id( (Id)i );
       if (caseInsensitiveEquals(toString(id),s)) {
          m_id = id;
          break;
       } 
   }
}


DataType::DataType(unsigned const n) : m_id(Base)
{
   if (n < Invalid) m_id = (Id)n;
}


unsigned DataType::toUInt() const
{
   return static_cast<unsigned>(m_id);
}


String DataType::toString() const
{
   return toString(m_id);
}


String DataType::toString(Id id)
{
   String s("Base");

   switch(id) {
      case Base:                      s = "Base";                      break;
      case ProjectGroup:              s = "ProjectGroup";              break;
      case Project:                   s = "Project";                   break;
      case MoleculeGroup:             s = "MoleculeGroup";             break;
      case Molecule:                  s = "Molecule";                  break;
      case GeometryGroup:             s = "GeometryGroup";             break;
      case Geometry:                  s = "Geometry";                  break;
      case StateGroup:                s = "StateGroup";                break;
      case State:                     s = "State";                     break;
      case CalculationGroup:          s = "CalculationGroup";          break;
      case Calculation:               s = "Calculation";               break;
      case PropertyGroup:             s = "PropertyGroup";             break;
      case Property:                  s = "Property";                  break;
      case Group:                     s = "Group";                     break;
      case Externals:                 s = "Externals";                 break;
      case BasisSet:                  s = "BasisSet";                  break;
      case Invalid:                   s = "Invalid";                   break;
   }

   return s;
}


bool DataType::operator==(DataType const& rhs) const 
{ 
   return m_id == rhs.m_id; 
}


bool DataType::operator!=(DataType const& rhs) const 
{ 
   return m_id != rhs.m_id; 
}


} // end namespace
