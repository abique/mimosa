add_library(rpc-kv-db-proto kv-db.pb.cc)

add_executable(rpc-kv-db kv-db.cc database.cc)
target_link_libraries(rpc-kv-db rpc-kv-db-proto mimosa-rpc)
