install(FILES
  sqlite.hh
  sqlite.hxx
  DESTINATION include/mimosa/sqlite)

add_library(mimosa-sqlite STATIC sqlite.cc)
target_link_libraries(mimosa-sqlite PUBLIC mimosa)

install(TARGETS mimosa-sqlite
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
