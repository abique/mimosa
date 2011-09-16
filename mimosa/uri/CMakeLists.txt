install(FILES
  normalize-path.hh
  parse-query.hh
  percent-encoding.hh
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri
  parse-query.cc
  percent-encoding.cc
  normalize-path.cc)

add_subdirectory(tests)

install(TARGETS mimosa-uri
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)