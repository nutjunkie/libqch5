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

/// The Schema determine the heirarchy of the data storage for a given ProjectFile.
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

      // Returns a path of DataTypes that leads to the given DataType.  
      // If the DataType is not found in the Schema, an empty list is 
      // returned;
      List<DataType> find(DataType const&) const;

      String serialize() const;
      bool deserialize(String const&);

      void print() const;

      bool operator==(Schema const& rhs) const;
      bool operator!=(Schema const& rhs) const { return !(*this == rhs); }



// DEPRECATE
// Note that this will return the first DataType encountered and
// does not handle the case where a given DataType appears more
// than once in the Schema.
DataType type(char const* path) const;
// Utility function that determines the depth of the DataType
// returns -1 if the DataType does not appear in the Schema.
int depth(DataType const&) const;
bool isValid(char const* path, DataType const&) const;
// Utility function that determines the depth of the path
// (essentially the number of '/' characters in the path.
int depth(char const* path) const;
      

   private:
      Tree m_tree;
};

} // end namespace


#endif
