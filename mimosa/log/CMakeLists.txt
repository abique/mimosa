install(FILES
  level.hh
  log.hh
  origin.hh
  DESTINATION include/mimosa/log/)

add_library(mimosa-log level.cc log.cc origin.cc)

target_link_libraries(mimosa-log mimosa-options)

install(TARGETS mimosa-log
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)