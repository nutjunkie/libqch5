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
      case Project:                   s = "Project";                   break;
      case ProjectGroup:              s = "ProjectGroup";              break;
      case Molecule:                  s = "Molecule";                  break;
      case Calculation:               s = "Calculation";               break;
      case CalculationGroup:          s = "CalculationGroup";          break;
   }

   return s;
}

DataType::id DataType::toId(String const& s)
{
   id 
   for (unsigned i = 0; i < MaxDataTypeId; ++i) {
   }
}

} // end namespace
