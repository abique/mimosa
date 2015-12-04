install(FILES
  base16-decoder.hh
  base16-encoder.hh
  base64-decoder.hh
  buffered-stream.hh
  buffer.hh
  copy.hh
  direct-fd-stream.hh
  fd-stream.hh
  filter.hh
  filter.hxx
  hash.hh
  html-escape.hh
  limited-stream.hh
  lzma.hh
  lzma-decoder.hh
  lzma-encoder.hh
  net-fd-stream.hh
  percent-encoder.hh
  stream.hh
  string-stream.hh
  tee-stream.hh
  tls-stream.hh
  zlib-encoder.hh
  DESTINATION include/mimosa/stream/)

add_subdirectory(tests)
