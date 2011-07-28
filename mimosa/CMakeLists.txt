install(FILES
  bind.hh
  function.hh
  init.hh
  main.hh
  non-copyable.hh
  ref-countable.hh
  ref-counted-ptr.hh
  DESTINATION include/mimosa/)

add_subdirectory(string)
add_subdirectory(runtime)
add_subdirectory(net)
add_subdirectory(http)