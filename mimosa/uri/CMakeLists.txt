install(FILES
  normalize-path.hh
  parse-query.hh
  percent-encoding.hh
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri url.cc)

add_subdirectory(tests)
