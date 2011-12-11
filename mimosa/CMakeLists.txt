install(FILES
  bind.hh
  function.hh
  init.hh
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
  stream/buffer.cc
  stream/buffered-stream.cc
  stream/compress-stream.cc
  stream/copy.cc
  stream/direct-fd-stream.cc
  stream/fd-stream.cc
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

  # http
  http/cookie.cc
  http/dispatch-handler.cc
  http/error-handler.cc
  http/fs-handler.cc
  http/method.cc
  http/mime-db.cc
  http/log.cc
  http/log-handler.cc
  http/redirect.cc
  http/request.cc
  http/request-reader.cc
  http/request-lexer.cc
  http/request-parser.cc
  http/response.cc
  http/response-writer.cc
  http/status.cc
  http/server.cc
  http/server-channel.cc

  # rpc
  rpc/basic-call.cc
  rpc/channel.cc
  rpc/server.cc
  rpc/service.cc
  rpc/service-map.cc

  # tpl
  tpl/abstract-value.cc
  tpl/cache.cc
  tpl/dict.cc
  tpl/filter-factory.cc
  tpl/include.cc
  tpl/list.cc
  tpl/log.cc
  tpl/parser.cc
  tpl/template.cc

  tpl/ast/empty.cc
  tpl/ast/repeated.cc
  tpl/ast/root.cc
  tpl/ast/text.cc
  tpl/ast/var.cc

  # core initialisation
  init.cc)
target_link_libraries(mimosa protobuf gnutls pthread rt)

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

add_subdirectory(tests)

install(TARGETS mimosa
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
