install(FILES
  flat.hh
  DESTINATION include/mimosa/flat/)

add_library(mimosa-flat flat.cc)
target_link_libraries(mimosa-flat mimosa)

install(TARGETS mimosa-flat
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
