install(FILES
  coding.hh
  cookie.hh
  dispatch-handler.hh
  error-handler.hh
  fs-handler.hh
  handler.hh
  method.hh
  log.hh
  log-handler.hh
  redirect.hh
  request.hh
  request-reader.hh
  response.hh
  response-writer.hh
  server-channel.hh
  server.hh
  status.hh
  DESTINATION include/mimosa/http/)

add_subdirectory(tests)
add_subdirectory(samples)
