install(FILES
  bind.hh
  function.hh
  init.hh
  main.hh
  non-copyable.hh
  ref-countable.hh
  ref-counted-ptr.hh
  DESTINATION include/mimosa/)

# just to make files appears on qtcreator
file(GLOB HEADERS *.hh *.hxx */*.hh */*.hxx)

add_library(mimosa init.cc ${HEADERS})
target_link_libraries(mimosa mimosa-runtime melon gnutls gflags)

add_subdirectory(container)
add_subdirectory(http)
add_subdirectory(net)
add_subdirectory(rpc)
add_subdirectory(runtime)
add_subdirectory(stream)
add_subdirectory(string)
add_subdirectory(sync)
add_subdirectory(uri)

add_subdirectory(tests)

install(TARGETS mimosa
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)