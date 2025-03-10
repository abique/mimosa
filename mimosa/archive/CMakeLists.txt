pkg_check_modules(LibArchive REQUIRED IMPORTED_TARGET libarchive)

add_library(mimosa-archive STATIC
  reader.cc
  writer.cc)

install(FILES
  entry.hh
  reader.hh
  writer.hh
  DESTINATION include/mimosa/archive/)

target_link_libraries(mimosa-archive PUBLIC mimosa PkgConfig::LibArchive)

install(TARGETS mimosa-archive
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
