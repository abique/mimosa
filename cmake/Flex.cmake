macro(mimosa_flex name)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.hh ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cc
    COMMAND flex ${name}.l
    DEPENDS ${name}.l
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  # set_source_files_properties(${name}.hh ${name}.cc
  #   PROPERTIES GENERATED true)
endmacro(mimosa_flex)
