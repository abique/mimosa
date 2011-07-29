install(FILES
  bind.hh
  function.hh
  init.hh
  main.hh
  non-copyable.hh
  ref-countable.hh
  ref-counted-ptr.hh
  DESTINATION include/mimosa/)

add_subdirectory(stream)
add_subdirectory(string)
add_subdirectory(uri)
add_subdirectory(runtime)
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(sync)

add_library(mimosa init.cc)
target_link_libraries(mimosa gnutls)
