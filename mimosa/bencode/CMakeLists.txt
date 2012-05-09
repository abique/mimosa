install(FILES
  bencode.hh
  DESTINATION include/mimosa/bencode/)

add_library(mimosa-bencode
  decoder.cc
  )
target_link_libraries(mimosa-bencode mimosa)

install(TARGETS mimosa-bencode
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
