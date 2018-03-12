/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "DataType.h"


namespace libqch5 {

String DataType::toString(Id id)
{
   String s;

   switch(id) {
      case Base:                      s = "BaseData";                  break;
      case Project:                   s = "Project";                   break;
      case ProjectGroup:              s = "ProjectGroup";              break;
      case Molecule:                  s = "Molecule";                  break;
      case Calculation:               s = "Calculation";               break;
      case CalculationGroup:          s = "CalculationGroup";          break;
      case Invalid:                   s = "Invalid";                   break;
   }

   return s;
}

DataType::Id DataType::toId(String const& s)
{
   for (unsigned i = 0; i < Invalid; ++i) {
       Id id( (Id)i );
       if (toString(id) == s) {
          return id;
          break;
       } 
   }

   return Invalid;
}

} // end namespace
