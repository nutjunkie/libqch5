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
      typedef st_tree::tree<DataType> Tree;
      typedef st_tree::tree<DataType>::node_type Node;

   public:
      Schema(DataType const root = DataType::ProjectGroup);

      Node& root() { return m_tree.root(); };
      Node& appendChild(String const& data);
      Node& appendChild(DataType const& id);

      bool isValid(char const* path, RawData const&) const;

      unsigned depth(DataType const& type) const;
      unsigned depth(char const* path) const;
      
      void print() const;

      bool operator==(Schema const& rhs) const;

   private:
      Tree m_tree;
};

} // end namespace


#endif
