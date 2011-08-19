mimosa_flex(request-lexer)
mimosa_bison(request-parser)

add_library(mimosa-http
  cookie.cc
  dispatch-handler.cc
  error-handler.cc
  fs-handler.cc
  status.cc
  server.cc
  server-channel.cc
  request.cc
  request-reader.cc
  request-lexer.cc
  request-parser.cc
  response.cc
  response-writer.cc
  )

target_link_libraries(mimosa-http mimosa-uri mimosa-stream mimosa-net)

add_subdirectory(tests)
add_subdirectory(samples)
