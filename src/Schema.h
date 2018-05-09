#ifndef LIBQCH5_SCHEMA_H
#define LIBQCH5_SCHEMA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Types.h"
#include "st_tree.h"
#include "DataType.h"


namespace libqch5 {

/// Schemas determine the heirarchy of the data storage for a given ProjectFile.
class Schema {

   public:
      typedef st_tree::tree<DataType> Tree;
      typedef st_tree::tree<DataType>::node_type Node;

   public:
      Schema(DataType const root = DataType::Base);

      Node& root() { return m_tree.root(); };

      Node& appendChild(DataType const& id);

      // We allow strings on the assumption that any unrecognised 
      // ContextData type will be handled generically.
      Node& appendChild(String const& data);

      // Note that this will return the first DataType encountered and
      // does not handle the case where a given DataType appears more
      // than onece in the Schema.
      DataType type(char const* path) const;
      
      String serialize() const;
      bool deserialize(String const&);

      void print() const;

// deprecate
bool isValid(char const* path, DataType const&) const;

      bool operator==(Schema const& rhs) const;
      bool operator!=(Schema const& rhs) const { return !(*this == rhs); }

   private:
      // Utility function that determines the depth of the path
      // (essentially the number of '/' characters in the path.
      int depth(char const* path) const;

      // Utility function that determines the depth of the DataType
      // returns -1 is the DataType does not appear in the Schema.
      int depth(DataType const&) const;

      Tree m_tree;
};

} // end namespace


#endif
