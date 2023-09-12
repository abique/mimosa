install(FILES
  copy.hh
  decoder.hh
  encoder.hh
  DESTINATION include/mimosa/bencode/)

add_library(mimosa-bencode STATIC
  copy.cc
  decoder.cc
  encoder.cc
  )
target_link_libraries(mimosa-bencode PUBLIC mimosa)

install(TARGETS mimosa-bencode
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
