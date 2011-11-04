install(FILES
  options.hh
  DESTINATION include/mimosa/options/)

add_library(mimosa-options options.cc)

install(TARGETS mimosa-options
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
