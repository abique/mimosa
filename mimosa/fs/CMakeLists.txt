install(FILES
  copy.hh
  dir-iterator.hh
  exists.hh
  find.hh
  move.hh
  rm.hh
  DESTINATION include/mimosa/fs/)

add_library(mimosa-fs
  copy.cc
  dir-iterator.cc
  exists.cc
  find.cc
  move.cc
  rm.cc
  )

install(TARGETS mimosa-fs
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
