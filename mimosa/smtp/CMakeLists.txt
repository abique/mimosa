install(FILES
  client.hh
  DESTINATION include/mimosa/smtp/)

add_library(mimosa-smtp
  client.cc
  mail.cc)
target_link_libraries(mimosa-smtp mimosa)

install(TARGETS mimosa-smtp
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(sample)