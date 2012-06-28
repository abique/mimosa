install(FILES
  base16-decoder.hh
  base16-encoder.hh
  buffered-stream.hh
  buffer.hh
  copy.hh
  direct-fd-stream.hh
  fd-stream.hh
  filter.hh
  gzip-stream.hh
  hash.hh
  hash.hxx
  html-escape.hh
  limited-stream.hh
  lzma.hh
  lzma-decoder.hh
  lzma-encoder.hh
  percent-encoder.hh
  stream.hh
  string-stream.hh
  tee-stream.hh
  tls-stream.hh
  zlib-encoder.hh
  DESTINATION include/mimosa/stream/)

add_library(mimosa-stream-lzma lzma.cc lzma-decoder.cc lzma-encoder.cc)
target_link_libraries(mimosa-stream-lzma mimosa lzma)

if(ZLIB_FOUND)
  add_library(mimosa-stream-zlib zlib-encoder.cc)
  target_link_libraries(mimosa-stream-zlib mimosa z)
endif()

add_subdirectory(tests)
