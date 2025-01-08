#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RmlUi::Core" for configuration "Release"
set_property(TARGET RmlUi::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RmlUi::Core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/rmlui.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/rmlui.dll"
  )

list(APPEND _cmake_import_check_targets RmlUi::Core )
list(APPEND _cmake_import_check_files_for_RmlUi::Core "${_IMPORT_PREFIX}/lib/rmlui.lib" "${_IMPORT_PREFIX}/bin/rmlui.dll" )

# Import target "RmlUi::Debugger" for configuration "Release"
set_property(TARGET RmlUi::Debugger APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RmlUi::Debugger PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/rmlui_debugger.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/rmlui_debugger.dll"
  )

list(APPEND _cmake_import_check_targets RmlUi::Debugger )
list(APPEND _cmake_import_check_files_for_RmlUi::Debugger "${_IMPORT_PREFIX}/lib/rmlui_debugger.lib" "${_IMPORT_PREFIX}/bin/rmlui_debugger.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
