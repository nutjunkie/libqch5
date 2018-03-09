#ifndef LIBQCH5_SCHEMA_H
#define LIBQCH5_SCHEMA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"
#include "st_tree.h"


namespace libqch5 {


/// Schemas determine the heirarchy of the data storage for a given ProjctFile.
/// We allow strings on the assumption that any unrecognised ContextData type can
/// be handled generically.
class Schema {

   public:
      typedef st_tree::tree<String> Tree;
      typedef st_tree::tree<String>::node_type Node;

   public:
      Schema(String const& root = "Root");

      Node& root() { return m_tree.root(); };
      Node& appendChild(String const& data);
      
      bool contains(String const&);
      unsigned depth(String const&);
      void print();

      bool operator==(Schema const& rhs);

   private:
      Tree m_tree;
};

} // end namespace


#endif
