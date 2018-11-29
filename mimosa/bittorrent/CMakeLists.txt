install(FILES
  torrent-descriptor.hh
  torrent-parser.hh
  DESTINATION include/mimosa/bittorrent/)

add_library(mimosa-bittorrent STATIC
  torrent-descriptor.cc
  torrent-descriptor.hh
  torrent-parser.cc
  torrent-parser.hh
  )
target_link_libraries(mimosa-bittorrent
  mimosa-bencode
  mimosa)

install(TARGETS mimosa-bittorrent
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
