install(FILES
  bind.hh
  function.hh
  init.hh
  init.doch
  non-copyable.hh
  ref-countable.hh
  ref-counted-ptr.hh
  DESTINATION include/mimosa/)

# just to make files appears on qtcreator
file(GLOB HEADERS *.hh *.hxx */*.hh */*.hxx)

mimosa_flex(http/request-lexer)
mimosa_bison(http/request-parser)

add_library(mimosa
  # Some kind of tricks to see headers in IDE like qt-creator.
  # I should remove them as Qt-creator is a crap and only emacs
  # does well.
  ${HEADERS}

  # command line options
  options/options.cc

  # runtime
  runtime/thread.cc
  runtime/thread-pool.cc

  # streams
  stream/base16-decoder.cc
  stream/base16-encoder.cc
  stream/buffer.cc
  stream/buffered-stream.cc
  stream/compress-stream.cc
  stream/copy.cc
  stream/direct-fd-stream.cc
  stream/fd-stream.cc
  stream/filter.cc
  stream/hash.cc
  stream/html-escape.cc
  stream/limited-stream.cc
  stream/net-fd-stream.cc
  stream/stream.cc
  stream/string-stream.cc
  stream/tls-stream.cc

  # logging
  log/level.cc
  log/log.cc
  log/origin.cc

  # networking stuff
  net/accept.cc
  net/connect.cc
  net/io.cc
  net/server.cc

  # uri
  uri/parse-query.cc
  uri/percent-encoding.cc
  uri/normalize-path.cc

  # core initialisation
  init.cc)
target_link_libraries(mimosa gnutls pthread rt)

add_subdirectory(container)
add_subdirectory(format)
add_subdirectory(runtime)
add_subdirectory(sync)
add_subdirectory(string)
add_subdirectory(options)
add_subdirectory(log)
add_subdirectory(uri)
add_subdirectory(stream)
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(rpc)
add_subdirectory(tpl)
add_subdirectory(sqlite)

add_subdirectory(tests)

install(TARGETS mimosa
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
