install(FILES
  decoder.hh
  encoder.hh
  DESTINATION include/mimosa/bencode/)

add_library(mimosa-bencode
  decoder.cc
  encoder.cc
  )
target_link_libraries(mimosa-bencode mimosa)

install(TARGETS mimosa-bencode
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
