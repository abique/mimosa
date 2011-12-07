install(FILES
  thread.hh
  thread-pool.hh
  time.hh
  yield.hh
  DESTINATION include/mimosa/runtime/)

add_library(mimosa-runtime
  thread.cc
  thread-pool.cc)

target_link_libraries(mimosa-runtime rt)

install(TARGETS mimosa-runtime
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
