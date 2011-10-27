install(FILES
  buffered-stream.hh
  buffer.hh
  compress-stream.hh
  copy.hh
  direct-fd-stream.hh
  fd-stream.hh
  gzip-stream.hh
  limited-stream.hh
  stream.hh
  tls-stream.hh
  DESTINATION include/mimosa/stream/)

add_library(mimosa-stream
  buffer.cc
  buffered-stream.cc
  compress-stream.cc
  copy.cc
  direct-fd-stream.cc
  fd-stream.cc
  limited-stream.cc
  net-fd-stream.cc
  stream.cc
  tls-stream.cc
)

target_link_libraries(mimosa-stream mimosa)

install(TARGETS mimosa-stream
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
