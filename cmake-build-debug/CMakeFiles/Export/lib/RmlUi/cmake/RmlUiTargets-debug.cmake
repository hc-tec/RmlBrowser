#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RmlCore" for configuration "Debug"
set_property(TARGET RmlCore APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RmlCore PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libRmlCore.so.5.1"
  IMPORTED_SONAME_DEBUG "libRmlCore.so.5"
  )

list(APPEND _IMPORT_CHECK_TARGETS RmlCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_RmlCore "${_IMPORT_PREFIX}/lib/libRmlCore.so.5.1" )

# Import target "RmlDebugger" for configuration "Debug"
set_property(TARGET RmlDebugger APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RmlDebugger PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libRmlDebugger.so.5.1"
  IMPORTED_SONAME_DEBUG "libRmlDebugger.so.5"
  )

list(APPEND _IMPORT_CHECK_TARGETS RmlDebugger )
list(APPEND _IMPORT_CHECK_FILES_FOR_RmlDebugger "${_IMPORT_PREFIX}/lib/libRmlDebugger.so.5.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
