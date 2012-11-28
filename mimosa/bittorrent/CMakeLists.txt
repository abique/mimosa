install(FILES
  torrent-info.hh
  torrent-parser.hh
  DESTINATION include/mimosa/bittorrent/)

add_library(mimosa-bittorrent
  torrent-info.cc
  torrent-parser.cc
  )
target_link_libraries(mimosa-bittorrent mimosa)

install(TARGETS mimosa-bittorrent
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
