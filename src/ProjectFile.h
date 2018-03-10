#ifndef LIBQCH5_PROJECTFILE_H
#define LIBQCH5_PROJECTFILE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

********************************************************************************/

#include "hdf5.h"
#include "Schema.h"
#include "Types.h"


namespace libqch5 {

class RawData;
class Schema;

class ProjectFile {

   public:
      enum Status { Closed, Open, Error };
      
      ProjectFile(char const* filePath);

	  /// Opens a new Project file and sets the given Schema.  If the file
	  /// exists a check is made to ensure the Schema match.
	  ProjectFile(char const* filePath, Schema const&);

      ~ProjectFile();

      Status status() const { return m_status; }

      String const& error() const { return m_error; }

      void put(char const* path, RawData const& data);
      void get(char const* path, RawData& data);

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

	  /// Attempts to read an existing Schema from the file, returning false
	  /// if none can be found.
      bool readSchema(Schema&);

	  /// Writes the Schema to file, returning false if a schema alread exists,
	  /// or if the write failed.
      bool writeSchema(Schema const&);

      String  m_error;
      hid_t   m_fileId;
      Status  m_status;

      Schema  m_schema;
};

} // end namespace

#endif
