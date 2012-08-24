install(FILES
  barrier.hh
  bind.hh
  cache.hh
  cache.hxx
  channel.hh
  cpu-foreach.hh
  condition.hh
  container-of.hh
  function.hh
  future.hh
  future.hxx
  init.doch
  init.hh
  intrusive-dlist.hh
  intrusive-slist.hh
  kvs.hh
  locker.hh
  mutex.hh
  non-copyable.hh
  recursive-mutex.hh
  ref-countable.hh
  ref-counted-ptr.hh
  shared-locker.hh
  shared-mutex.hh
  singleton.hh
  singleton.hxx
  spin-lock.hh
  spin-lock.hxx
  string-ref.hh
  thread-pool.hh
  thread.hh
  time.hh
  trie.hh
  trie.hxx
  unique-locker.hh
  uptime.hh
  yield.hh
  DESTINATION include/mimosa/)

# just to make files appears on qtcreator
file(GLOB HEADERS *.hh *.hxx */*.hh */*.hxx)

add_library(mimosa
  # Some kind of tricks to see headers in IDE like qt-creator.
  # I should remove them as Qt-creator is a crap and only emacs
  # does well.
  ${HEADERS}

  # command line options
  options/options.cc

  # runtime
  cpu-foreach.cc
  thread.cc
  thread-pool.cc
  uptime.cc

  # sync
  barrier.cc

  # streams
  stream/base16-decoder.cc
  stream/base16-encoder.cc
  stream/buffer.cc
  stream/buffered-stream.cc
  stream/copy.cc
  stream/direct-fd-stream.cc
  stream/fd-stream.cc
  stream/filter.cc
  stream/hash.cc
  stream/html-escape.cc
  stream/limited-stream.cc
  stream/percent-encoder.cc
  stream/net-fd-stream.cc
  stream/stream.cc
  stream/string-stream.cc
  stream/tee-stream.cc
  stream/tls-stream.cc
  stream/zlib-encoder.cc
  stream/lzma.cc
  stream/lzma-decoder.cc
  stream/lzma-encoder.cc

  # format
  format/print.cc

  # logging
  log/level.cc
  log/log.cc
  log/origin.cc

  # networking stuff
  net/accept.cc
  net/connect.cc
  net/io.cc
  net/print.cc
  net/server.cc

  # uri
  uri/parse-query.cc
  uri/percent-encoding.cc
  uri/normalize-path.cc

  # core initialisation
  init.cc
  priviledge-drop.cc)
target_link_libraries(mimosa ${GNUTLS_LIBRARY} lzma ${ZLIB_LIBRARY} pthread rt)

add_subdirectory(format)
add_subdirectory(options)
add_subdirectory(log)
add_subdirectory(uri)
add_subdirectory(stream)
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(rpc)
add_subdirectory(tpl)
add_subdirectory(sqlite)
add_subdirectory(bencode)
add_subdirectory(flat)
add_subdirectory(fs)

add_subdirectory(tests)

install(TARGETS mimosa
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
