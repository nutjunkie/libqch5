/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Schema.h"


namespace libqch5 {

Schema::Schema(String const& root)
{
    m_tree.insert(root);
}


bool Schema::contains(String const&) 
{
   return true;
}


Schema::Node& Schema::appendChild(String const& data) 
{ 
   return m_tree.root().appendChild(data); 
};


} // end namespace
