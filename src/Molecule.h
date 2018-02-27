/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Node.h"

namespace libqch5 {

class Molecule : public Node {

   public:
      Molecule() : Node(Node::Molecule) { }

   private:
      unsigned m_nAtoms;
};

} // end namespace


