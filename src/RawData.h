#ifndef LIBQCH5_RAWDATA_H
#define LIBQCH5_RAWDATA_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include "hdf5.h"
#include "Array.h"
#include "Types.h"
#include "DataType.h"
#include "Attributes.h"


namespace libqch5 {

class RawData {

   friend class ProjectFile;

   public:
       RawData(String const& label = "Untitled", 
          DataType::Id const& type = DataType::Base) 
        : m_label(label), m_type(type) { }

       ~RawData();

       String const& label()   const { return m_label; }
       DataType const& dataType() const { return m_type; }

       template <typename T>
       void setAttribute(String const& name, T value) {
          m_attributes.set(name, value);
       }

       template <typename T>
       bool getAttribute(String const& name, T value) {
          return m_attributes.get(name, value);
       }


       /// Allocates a new Array<D,T> of Size and appends it to the list of
       /// known data.
       template < size_t D, typename T>
       Array<D, T>& createArray(typename Array<D, T>::Size const& size)
       {
          Array<D, T>* d(new Array<D,T>(size));
          DEBUG("Allocated array " << d);
          m_arrays.push_back(d);
          return *d;
       }

       // Convenience functions for creating arrays of rank 1-3
       template <typename T = double>
       Array<1, T>& createArray(size_t n) 
       {
          typename Array<1, T>::Size size = { n };
          return createArray<1,T>(size);
       }

       template <typename T = double>
       Array<2, T>& createArray(size_t n1, size_t n2) 
       {
          typename Array<2, T>::Size size = { n1, n2 };
          return createArray<2,T>(size);
       }

       template <typename T = double>
       Array<3, T>& createArray(size_t n1, size_t n2, size_t n3) 
       {
          typename Array<3, T>::Size size = { n1, n2, n3 };
          return createArray<3,T>(size);
       }


   protected:
       bool write(hid_t gid) const;

	   /// Attempts to read the data contained in the gid into this object.  It
	   /// is assumed the label has been set appropriately before calling this
	   /// function.
       bool read(hid_t gid);

   private:
       bool write(hid_t fid, char const* path, hid_t tid, size_t rank, 
          hsize_t const* dimensions, void const* data) const;

       bool read(hid_t gid, char const* path);

       String             m_label;
       DataType           m_type;

       List< ArrayBase*>  m_arrays;

       Attributes  m_attributes;
};

} // end namespace

#endif
