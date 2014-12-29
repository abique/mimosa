install(FILES
  normalize-path.hh
  parse-query.hh
  percent-encoding.hh
  scheme.hh
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri url.cc)
target_link_libraries(mimosa-uri mimosa)

add_subdirectory(tests)
