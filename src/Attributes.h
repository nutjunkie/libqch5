#ifndef LIBQCH5_ATTRIBUTES_H
#define LIBQCH5_ATTRIBUTES_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "hdf5.h"
#include "Types.h"


namespace libqch5 {

/// A wrapper around Attribute maps of various types.  Also takes care of writing
/// attributes to file.
class Attributes {

   public:
      void set(String const& key, int value) {
         m_attributesInt[key] =  value;
      }

      void set(String const& key, unsigned value) {
         m_attributesUInt[key] =  value;
      }

      void set(String const& key, double value) {
         m_attributesDouble[key] =  value;
      }

      void set(String const& key, String const& value) {
         m_attributesString[key] =  value;
      }

      /// If found, sets value to the value of the attribute 
      /// and returns true, otherwise, returns false.
      bool get(String const& key, int& value) const {
         StringMap<int>::const_iterator iter(m_attributesInt.find(key));
         if (iter != m_attributesInt.end()) {
            value = iter->second;
            return true;
         } 
         return false;
      }

      /// If found, sets value to the value of the attribute 
      /// and returns true, otherwise, returns false.
      bool get(String const& key, unsigned& value) const {
         StringMap<unsigned>::const_iterator iter(m_attributesUInt.find(key));
         if (iter != m_attributesUInt.end()) {
            value = iter->second;
            return true;
         } 
         return false;
      }

      /// If found, sets value to the value of the attribute 
      /// and returns true, otherwise, returns false.
      bool get(String const& key, double& value) const {
         StringMap<double>::const_iterator iter(m_attributesDouble.find(key));
         if (iter != m_attributesDouble.end()) {
            value = iter->second;
            return true;
         } 
         return false;
      }

      /// If found, sets value to the value of the attribute 
      /// and returns true, otherwise, returns false.
      bool get(String const& key, String& value) const {
         StringMap<String>::const_iterator iter(m_attributesString.find(key));
         if (iter != m_attributesString.end()) {
            value = iter->second;
            return true;
         } 
         return false;
      }

      // Sets the attributes to the given object ID 
      bool write(hid_t oid, char const* label) const;
      bool read(hid_t oid, char const* label);
      void clear();

   protected:
      StringMap<int>      m_attributesInt;
      StringMap<unsigned> m_attributesUInt;
      StringMap<double>   m_attributesDouble;
      StringMap<String>   m_attributesString;
};

} // end namespace

#endif
