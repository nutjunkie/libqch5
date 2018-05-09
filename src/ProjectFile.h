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

class ProjectFile {

   public:
      enum IOStat { Closed, Open, Error };
      enum IOMode { New, Old, Overwrite };
      
      // Initializes a new ProjectFile with the given file path.  For files with
      // IOMode set to New or Overwrite, the Schema should be passed in to the
      // constructor.  For exisiting (Old) files the Schema is read in from the 
      // file.  If a Schema is also specified, a  check is made to ensure the
      // Schemas match.
	  ProjectFile(char const* filePath, IOMode const = Old, Schema const& = Schema());

      ~ProjectFile();

      String const& error() const { return m_error; }

      // Writes the given data object as a child of the path
      void write(char const* path, RawData const& data);

      // Reads the given data object as a child of the path
      bool read( char const* path, RawData& data);

      // Checks if the given path is valid for the given data
      bool isValid(char const* path, RawData const& data) const;


      // Checks if the path currently exists in the file
      bool pathExists(char const* path) const;

      bool addGroup(char const* path);



   private:
      IOStat ioStat() const { return m_ioStat; }

	  void open(char const* filePath, IOMode const = Old, Schema const& = Schema());

      /// Closes the attached file, updating m_ioStat.
      void close();

	  /// Attempts to read an existing Schema from the file, returning false
	  /// if none can be found.
      bool readSchema(Schema&);

	  /// Writes the Schema to file, returning false if a schema alread exists,
	  /// or if the write failed.
      bool writeSchema(Schema const&);

      String  m_error;
      hid_t   m_fileId;
      IOStat  m_ioStat;
      Schema  m_schema;
};

} // end namespace

#endif
