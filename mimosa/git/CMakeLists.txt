install(FILES
  archive.hh
  blob.hh
  commit.hh
  diff.hh
  patch.hh
  repository.hh
  revwalk.hh
  tree-entry.hh
  tree.hh
  DESTINATION include/mimosa/git/)

add_library(mimosa-git
  archive.cc
  reference.cc
  repository.cc
  revwalk.cc
  tree.cc
  )

target_link_libraries(mimosa-git
  mimosa-archive
  mimosa)

install(TARGETS mimosa-git
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
