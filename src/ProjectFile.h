#ifndef LIBQCH5_PROJECTFILE_H
#define LIBQCH5_PROJECTFILE_H
/*******************************************************************************

  This file is part of libqc-hdf a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "H5Cpp.h"
#include "Types.h"

namespace libqch5 {

class ProjectFile {

   public:
      enum Status { Closed, Open };
      
      ProjectFile(String const& filePath);

      ~ProjectFile();

      Status status() const { return m_status; }

      String const& error() const { return m_error; }

      void put(String const& path, Data const&);

   private:
      /// Sets-up the appropriate hierarchy for a new project file
      bool initGroupHierarchy();

	  /// Initializes a new project file.
      void init(String const& filePath);

	  /// Attempts to connect to an existing Project file, updating m_status on
	  /// success.
      void open(String const& filePath);

      /// Closes the attached file, updating m_status.
      void close();

      String  m_error;
      H5File* m_file;
      Status  m_status;
};

} // end namespace

#endif
