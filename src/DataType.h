#ifndef LIBQCH5_DATATYPE_H
#define LIBQCH5_DATATYPE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/


namespace libqch5 {

class DataType {

   public:
      enum Id { Invalid = 0,
                ProjectGroup,
                Project, 
                MoleculeGroup,
                Molecule,
                GeometryGroup,
                Geometry,
                StateGroup,
                State,
                CalculationGroup,
                Calculation,
                PropertyGroup,
                Property,
                MaxDataTypeId 
              };

      static Id toId(String const&);

      static String toString(Id);

String Schema::toString(DataType dataType)
{
   String s;

   switch(dataType) {
      case Project:      s = "Project";      break;
      case Molecule:     s = "Molecule";     break;
      case Calculation:  s = "Calculation";  break;
   }

   return s;
}
};

} // end namespace


#endif
