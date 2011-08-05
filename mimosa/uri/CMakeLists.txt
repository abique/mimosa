install(FILES
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri
  percent-encoding.cc
  normalize-path.cc)

add_subdirectory(tests)
