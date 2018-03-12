#ifndef LIBQCH5_SCHEMA_H
#define LIBQCH5_SCHEMA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Types.h"
#include "st_tree.h"
#include "DataType.h"


namespace libqch5 {

class RawData;

/// Schemas determine the heirarchy of the data storage for a given ProjctFile.
/// We allow strings on the assumption that any unrecognised ContextData type can
/// be handled generically.
class Schema {

   public:
      typedef st_tree::tree<String> Tree;
      typedef st_tree::tree<String>::node_type Node;

   public:
      Schema(String const& root = "Projects");

      Node& root() { return m_tree.root(); };
      Node& appendChild(String const& data);

      bool isValid(char const* path, RawData const&) const;
      
      bool contains(String const&) const;
      unsigned depth(char const* path) const;
      unsigned depth(DataType::Id id) const;
      void print() const;

      bool operator==(Schema const& rhs);

   private:
      Tree m_tree;
};

} // end namespace


#endif
