install(FILES
  dir-iterator.hh
  find.hh
  DESTINATION include/mimosa/fs/)

add_library(mimosa-fs
  dir-iterator.cc
  find.cc
  )

install(TARGETS mimosa-fs
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
