install(FILES
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri
  parse-query.cc
  percent-encoding.cc
  normalize-path.cc)

add_subdirectory(tests)
