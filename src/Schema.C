/*******************************************************************************

  This file is part of libqchd5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "Schema.h"
#include "Debug.h"
#include "x_common.h"
#include <sstream>


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


bool Schema::isValid(char const* path, DataType const& dataType) const
{
   bool valid(depth(path) == depth(dataType));;
   String s(valid ? "true" : "false");
   DEBUG("Schema checking " << path << " against type " << dataType.toString() << " - " << s);

   return valid;
}


int Schema::depth(DataType const& type) const
{
   Tree::const_iterator iter;
   for (iter = m_tree.bf_begin(); iter != m_tree.bf_end(); ++iter) {
       // DEBUG("Depth test ply(" << iter->ply() << ") " << iter->data().toString() );
       if (iter->data() == type) return iter->ply();
   }

   return -1;
}


int Schema::depth(char const* path) const
{
   String s(path);
   if (s.back()  == '/') s.pop_back();   // strip any trailing '/'
   int d(std::count(s.begin(), s.end(), '/'));
   if (s.front() == '/') --d;            // ignore any inital '/'

   return d;
}


String Schema::serialize() const 
{
   std::stringstream ss;
   unsigned depth(0);
   ss << "[";

   Tree::const_df_pre_iterator iter;
   for (iter = m_tree.df_pre_begin(); iter != m_tree.df_pre_end(); ++iter) {
        if (iter->ply() == depth) {
           ss << " ";
           ss << iter->data().toString();

        }else if (iter->ply() < depth) {
           while (depth > iter->ply()) { ss << " ] "; --depth; }
           ss << iter->data().toString();

        }else if (iter->ply() > depth) {
           ss << " [ ";
           ++depth;
           ss << iter->data().toString();
        }
        // ss << iter->ply();  // append depth for debugging
   }

   while (depth > 0) { ss << " ]"; --depth; }
   ss << " ]";

   return ss.str();
}


bool Schema::deserialize(String const& s)
{
   bool ok(true);

   std::stringstream ss(s);
   String token;

   m_tree.clear();
   Node* parent(0);
   Node* child(0);

   while (std::getline(ss, token, ' ')) {
      if (token == "[") {
         if (child != 0) {
            parent = child;
         }

      }else if (token == "]") {
         if (!parent->is_root()) {
            parent = &parent->parent();
         }

      }else if (!token.empty()) {
         if (parent == 0) {
            m_tree.insert(DataType(token));
            parent = &m_tree.root();
         }else {
            child = &parent->appendChild(DataType(token));
         }
      }
   }

   return ok;
}


void Schema::print() const
{
   DEBUG("Schema hierarchy:");
   // This is an alternative print strategy
   //serialize_indented(m_tree.df_pre_begin(), m_tree.df_pre_end(), std::cout);
   serialize_indented(m_tree, std::cout, 3);
}


bool Schema::operator==(Schema const& rhs) const
{
   return m_tree == rhs.m_tree;
}

} // end namespace
