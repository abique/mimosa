add_library(mimosa-stream
  buffer.cc
  buffered-stream.cc
  compress-stream.cc
  direct-fd-stream.cc
  fd-stream.cc
  limited-stream.cc
  stream.cc
)

target_link_libraries(mimosa-stream mimosa)
