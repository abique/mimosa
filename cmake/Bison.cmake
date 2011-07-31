macro(mimosa_bison name)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${name}.hh ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cc
    COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/${name}.y
    DEPENDS ${name}.y
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(mimosa_bison)
