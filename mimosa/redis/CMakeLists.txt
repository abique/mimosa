install(FILES
  context.hh
  log.hh
  redis.hh
  reply.hh
  DESTINATION include/mimosa/redis/)

add_library(mimosa-redis STATIC log.cc)
target_link_libraries(mimosa-redis PUBLIC mimosa)
