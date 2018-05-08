#ifndef LIBQCH5_PROJECTFILE_H
#define LIBQCH5_PROJECTFILE_H
/*******************************************************************************

  This file is part of libqch5 a data file format for managing quantum 
  chemistry projects.

  Copyright (C) 2018 Andrew Gilbert

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
      enum IOStat { New, Old, Overwrite };
      
      // Initializes a new ProjectFile with the given file path.  For files with
      // IOMode set to New or Overwrite, the Schema should be passed in to the
      // constructor.  For exisiting (Old) files the Schema is read in from the 
      // file.  If a Schema is also specified, a  check is made to ensure the
      // Schemas match.
	  ProjectFile(char const* filePath, IOStat const = Old, Schema const& = Schema());

      ~ProjectFile();

	  void open(char const* filePath, IOStat const = Old);

      Status status() const { return m_status; }
      String const& error() const { return m_error; }

      // Writes the given data object as a child of the path
      void write(char const* path, RawData const& data);

      // Reads the given data object as a child of the path
      void read( char const* path, RawData& data);

      // Checks if the path currently exists in the file
      bool pathExists(char const* path) const;

      bool addGroup(char const* path);

      bool isValid(char const* path, RawData const& data) const;

   private:
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
