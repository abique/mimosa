mimosa_flex(request-lexer)
mimosa_bison(request-parser)

add_library(mimosa-http
  cookie.cc
  dispatch-handler.cc
  status.cc
  server.cc
  request.cc
  request-lexer.cc
  request-parser.cc
  response-writer.cc
  )

target_link_libraries(mimosa-http mimosa-uri mimosa-stream mimosa-net)

add_subdirectory(tests)
