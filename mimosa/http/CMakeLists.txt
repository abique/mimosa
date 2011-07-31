mimosa_flex(request-lexer)
mimosa_bison(request-parser)

add_library(mimosa-http
  dispatch-handler.cc
  status.cc
  server.cc
  request.cc
  request-lexer.cc
  request-parser.cc
  )

target_link_libraries(mimosa-http mimosa-stream mimosa-net)

add_subdirectory(tests)