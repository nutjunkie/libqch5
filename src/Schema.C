/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Schema.h"
#include "x_common.h"
#include "RawData.h"


namespace libqch5 {

Schema::Schema(DataType const root)
{
    m_tree.insert(root);
}


Schema::Node& Schema::appendChild(DataType const& id) 
{ 
   return m_tree.root().appendChild(id); 
};


Schema::Node& Schema::appendChild(String const& type) 
{ 
   return m_tree.root().appendChild(DataType(type)); 
};


bool Schema::isValid(char const* path, RawData const& data) const
{
   DEBUG("Schema checking " << path);
   bool valid(true);
   String type(data.dataType().toString());
   unsigned d(depth(path));
   DEBUG("Depth of data " << d);
   String p(path);
   
   DEBUG("Testing type: "   << type << " " << data.dataType());
   return valid;
}


unsigned Schema::depth(DataType const& type) const
{
   Tree::const_iterator iter;
   for (iter = m_tree.bf_begin(); iter != m_tree.bf_end(); ++iter) {
        std::cout << iter->data().toString() << " ply(" << iter->ply() << ")" << "\n";
        if (iter->data() == type) break;
    }

   return iter->ply();
}


unsigned Schema::depth(char const* path) const
{
   String s(path);
   if (s.back()  == '/') s.pop_back();  // strip trailing '/'
   if (s.front() != '/') DEBUG("WARN: Data path not referenced to root");

   return std::count(s.begin(), s.end(), '/');
}






bool Schema::contains(String const&)  const
{
   return true;
}



void Schema::print() const
{
   std::cout << "\ntree<>::bf_iterator\n";
   serialize_indented(m_tree.df_pre_begin(), m_tree.df_pre_end(), std::cout);
   serialize_indented(m_tree, std::cout, 3);
}


bool Schema::operator==(Schema const& rhs) const
{
   return m_tree == rhs.m_tree;
}

} // end namespace
