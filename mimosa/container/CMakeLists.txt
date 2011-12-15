install(FILES
  cache.hh
  cache.hxx
  intrusive-slist.hh
  intrusive-dlist.hh
  kvs.hh
  singleton.hh
  singleton.hxx
  DESTINATION include/mimosa/container)

add_subdirectory(tests)
