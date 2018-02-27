/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/


namespace libqch5 {

/// 
class Geometry {

   public:
      void append(Project const&);
      void append(Component const&)

   private:
     unsigned m_nCom;
};

} // end namespace


