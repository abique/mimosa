add_subdirectory(tests)

install(FILES
  abstract-value.hh
  cache.hh
  dict.hh
  include.hh
  list.hh
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
