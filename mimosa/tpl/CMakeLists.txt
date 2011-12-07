add_library(mimosa-tpl
  abstract-value.cc
  template.cc
  parser.cc

  ast/empty.cc
  ast/repeated.cc
  ast/root.cc
  ast/text.cc
  ast/var.cc)

target_link_libraries(mimosa-tpl mimosa-log mimosa-stream)

add_subdirectory(tests)
