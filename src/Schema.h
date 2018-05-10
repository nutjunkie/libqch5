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


   private:
      Tree m_tree;
};

} // end namespace

#endif
