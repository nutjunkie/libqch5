#ifndef LIBQCH5_PROJECTFILE_H
#define LIBQCH5_PROJECTFILE_H
/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "hdf5.h"
#include "Types.h"

namespace libqch5 {

class Data;

class ProjectFile {

   public:
      enum Status { Closed, Open, Error };
      
      ProjectFile(char const* filePath);

      ~ProjectFile();

      Status status() const { return m_status; }

      String const& error() const { return m_error; }

      void put(char const* path, Data const& data);

   private:
      /// Sets-up the appropriate hierarchy for a new project file
      void initGroupHierarchy();

	  /// Initializes a new project file.
      void init(char const* filePath);

	  /// Attempts to connect to an existing Project file, updating m_status on
	  /// success.
      void open(char const* filePath);

      /// Closes the attached file, updating m_status.
      void close();

      String  m_error;
      hid_t   m_fileId;
      Status  m_status;
};

} // end namespace

#endif
