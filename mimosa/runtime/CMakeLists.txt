install(FILES
  fiber.hh
  time.hh
  yield.hh
  DESTINATION include/mimosa/runtime/)

add_library(mimosa-runtime
  fiber.cc)

target_link_libraries(mimosa-runtime mimosa)
