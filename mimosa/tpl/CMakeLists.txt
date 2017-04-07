install(FILES
  abstract-value.hh
  append-helper.hh
  cache.hh
  dict.hh
  include.hh
  list.hh
  log.hh
  map.hh
  parser.hh
  template.hh
  value.hh
  value.hxx

  DESTINATION include/mimosa/tpl/)

install(FILES
  ast/empty.hh
  ast/repeated.hh
  ast/root.hh
  ast/text.hh
  ast/var.hh
  ast/node.hh

  DESTINATION include/mimosa/tpl/ast/)

add_library(mimosa-tpl STATIC
  abstract-value.cc
  cache.cc
  dict.cc
  filter-factory.cc
  include.cc
  list.cc
  log.cc
  parser.cc
  template.cc

  ast/empty.cc
  ast/repeated.cc
  ast/root.cc
  ast/text.cc
  ast/var.cc
  )
target_link_libraries(mimosa-tpl mimosa)

install(TARGETS mimosa-tpl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
