install(FILES
  decoder.hh
  encoder.hh
  DESTINATION include/mimosa/json/)

add_library(mimosa-json
  encoder.cc
  decoder.cc
  )
target_link_libraries(mimosa-json mimosa)

install(TARGETS mimosa-json
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
