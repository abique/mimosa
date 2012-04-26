install(FILES
  base16-decoder.hh
  base16-encoder.hh
  buffered-stream.hh
  buffer.hh
  compress.hh
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
  DESTINATION include/mimosa/stream/)

add_subdirectory(tests)
