#ifndef LIBQCH5_ARRAY_H
#define LIBQCH5_ARRAY_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

********************************************************************************/

#include <array>
#include "hdf5.h"
#include "H5Utils.h"

#include "Debug.h"


namespace libqch5 {

/** \brief Polymorphic base for the Array class.  Makes available various data
           required for RawData to write the array to file.
 **/

class ArrayBase {

    friend class RawData;

    public: 
       virtual ~ArrayBase() { }
       virtual size_t rank() const = 0;

    protected:
       virtual hid_t h5DataType() const = 0;
       virtual void* buffer() = 0;
       virtual void const* buffer() const = 0;
       virtual size_t const* dimensions() = 0;
};


/** \brief Simple arbitrary rank array class.  Underlying data is stored as a
           single vector in column major format.

    \usage Use the Size typedef to give the size of each dimesion:

           Array<2>::Size size = { 5, 6 };
           Array<2> array(size);

           Use the Index typdef for indexing elements of the array:

           Array<2>::Index idx;

           for (idx[1] = 0; idx[1] < size.dim(1); ++idx[1]) {
               for (idx[0] = 0; idx[0] < size.dim(0); ++idx[0]) {
                   array(idx) = 0.0;
               }
           }

           array({3,4}) = 3.1415;
        
    \param D the rank of the array
    \param T the type of data stored in the array
 **/

template < size_t D, typename T = double >
class Array : public ArrayBase {

   public:

      typedef std::array<size_t, D> Size;
      typedef std::array<size_t, D> Index;

      Array(Size size) : m_data(0), m_size(size), m_length(0)
      { 
         resize(size);
      }

      ~Array() 
      { 
         if (m_data) delete m_data;
      }

	  /// Resizes the Array to the given Size after deleting any exisiting
	  /// allocated memory.  The contents of the Array are undefined after a
	  /// resize, if zero initialization is required, use the clear() function.
      void resize(Size size) 
      { 
         m_size = size;
         size_t n(1);

         for (size_t i = 0; i < m_size.size(); ++i) {
             m_offsets[i] = n;
             n *= m_size[i];
         }

         if (m_length != n) {
            m_length = n;
            if (m_data) delete [ ] m_data;
            m_data = new T[n*sizeof(T)];
         }
      }

      /// Initializes the Array buffer to zero
      void clear()
      {
         if (m_data) memset(m_data, 0, m_length*sizeof(T));
      }

      hid_t h5DataType() const { return H5DataType(T()); }

      size_t dim(size_t n) const { return (n < D ? m_size[n] : 0); }

      size_t rank() const { return D; }

      size_t length() const { return m_length; }

      Size const& dims() const { return m_size; }

      // Allows faster direct access to the data buffer
      T& operator[](size_t i) { return m_data[i]; }
      T const& operator[](size_t i) const { return m_data[i]; }

      // Column major index access
      T& operator()(Index d) 
      {
         size_t offset(0);
         for (size_t i = 0; i < D; ++i) {
             offset += d[i]*m_offsets[i];
         }

         return m_data[offset];
      }

      // Debug function, fills array with offset values;
      void fill()
      {
         T value(0);
         for (size_t i = 0; i < m_length; ++i) {
             m_data[i] = value++;
         }
      }

      void dump() 
      {
         std::cout << "Sizes (";
         for (size_t i = 0; i < m_size.size(); ++i) {
             std::cout << " " << m_size[i];
         }
         std::cout << ")" <<  std::endl;
      }


   protected:
      void* buffer() { return m_data; }
      void const* buffer() const { return m_data; }

      size_t*  dimensions() { return m_size.data(); }

   private:
      T*       m_data;
      Size     m_size;
      Size     m_offsets; // used for computing offset into m_data
      size_t   m_length;
};

} // end namespace

#endif
