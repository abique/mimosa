add_library(mimosa-tpl
  abstract-value.cc
  dict.cc
  list.cc
  parser.cc
  template.cc

  ast/empty.cc
  ast/repeated.cc
  ast/root.cc
  ast/text.cc
  ast/var.cc)

target_link_libraries(mimosa-tpl mimosa-log mimosa-stream)

add_subdirectory(tests)

install(FILES
  abstract-value.hh
  dict.hh
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

install(TARGETS mimosa-tpl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
