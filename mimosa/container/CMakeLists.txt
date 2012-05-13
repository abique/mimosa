install(FILES
  cache.hh
  cache.hxx
  intrusive-slist.hh
  intrusive-dlist.hh
  kvs.hh
  trie.hh
  trie.hxx
  DESTINATION include/mimosa/container)

add_subdirectory(tests)
