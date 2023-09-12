install(FILES
  normalize-path.hh
  parse-query.hh
  percent-encoding.hh
  scheme.hh
  url.hh
  DESTINATION include/mimosa/uri/)

add_library(mimosa-uri STATIC url.cc)
target_link_libraries(mimosa-uri PUBLIC mimosa)

add_subdirectory(tests)

install(TARGETS mimosa-uri
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
