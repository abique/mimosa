install(FILES
  client.hh
  mail.hh
  DESTINATION include/mimosa/smtp/)

add_library(mimosa-smtp STATIC
  client.cc
  mail.cc)
target_link_libraries(mimosa-smtp PUBLIC mimosa)

install(TARGETS mimosa-smtp
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(sample)
