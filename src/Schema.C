/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "Schema.h"
#include "x_common.h"
#include "RawData.h"


namespace libqch5 {

Schema::Schema(String const& root)
{
    m_tree.insert(root);
}


bool Schema::contains(String const&)  const
{
   return true;
}


unsigned Schema::depth(char const* path) const
{
   String s(path);
   if (s.back()  == '/') s.pop_back();  // strip trailing '/'
   if (s.front() != '/') DEBUG("WARN: Data path not referenced to root");

   return std::count(s.begin(), s.end(), '/');
}


unsigned Schema::depth(DataType::Id id) const
{
   String s(DataType::toString(id));
   serialize_indented(m_tree.df_pre_begin(), m_tree.df_pre_end(), std::cout);

   Tree::const_iterator iter;
   for (iter = m_tree.bf_begin(); iter != m_tree.bf_end(); ++iter) {
        std::cout << iter->data() << " ply(" << iter->ply() << ")" << "\n";
        if (iter->data() == s) break;
    }

   return iter->ply();
}




Schema::Node& Schema::appendChild(String const& data) 
{ 
   return m_tree.root().appendChild(data); 
};


bool Schema::isValid(char const* path, RawData const& data) const
{
   DEBUG("Schema checking " << path);
   bool valid(true);
   String type(DataType::toString( data.dataType() ));
   unsigned d(depth(path));
   DEBUG("Depth of data " << d);
   String p(path);
   
   DEBUG("Testing type: "   << type << " " << data.dataType());
   return valid;
}


void Schema::print() const
{
   std::cout << "\ntree<>::bf_iterator\n";
   serialize_indented(m_tree.df_pre_begin(), m_tree.df_pre_end(), std::cout);
   serialize_indented(m_tree, std::cout, 3);
}


bool Schema::operator==(Schema const& rhs)
{
   return m_tree == rhs.m_tree;
}

} // end namespace
