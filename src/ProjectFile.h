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
      // Error status should be set when the HDF5 file is open, but may be
      // in an invalid (recoverable?) state.
      enum IOStat { Closed, Open, Error };
      enum IOMode { New, Old, Overwrite };
      
      // Initializes a new ProjectFile with the given file path.  For files with
      // IOMode set to New or Overwrite, the Schema should be passed in to the
      // constructor.  For exisiting (Old) files the Schema is read in from the 
      // file.  If a Schema is also specified, a check is made to ensure matching
      // Schemata.
	  ProjectFile(char const* filePath, IOMode const = Old, Schema const& = Schema());

      ~ProjectFile();

      bool isOpen() const { return m_ioStat == Open; }
      String const& error() const { return m_error; }


      // Writes the given data object as a child of the path
      bool write(char const* path, RawData const& data);

      bool write(RawData const& data);

      // Reads the given data object as a child of the path
      bool read( char const* path, RawData& data);

      // Adds the specified group
      bool addGroup(char const* path, DataType const& = DataType(DataType::Group));


   private:
      // Checks if the path currently exists in the file
      bool pathExists(char const* path) const;

      // Performs a check to see if the DataType can be written to the group
      // given by path.  
      bool pathCheck(char const* path, DataType const&) const;

      DataType getDataType(char const* path) const;

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
