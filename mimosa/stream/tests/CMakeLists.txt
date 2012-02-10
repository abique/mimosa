mimosa_test(mimosa-stream
  tst-compress-stream.cc
  tst-filters.cc
  tst-hash.cc
  tst-html-escape.cc)

add_executable(lzma-encoder EXCLUDE_FROM_ALL lzma-encoder.cc)
target_link_libraries(lzma-encoder mimosa lzma)

add_executable(lzma-decoder EXCLUDE_FROM_ALL lzma-decoder.cc)
target_link_libraries(lzma-decoder mimosa lzma)
