protoc_mimosa(kv-db)

add_library(rpc-kv-db-proto kv-db.pb.cc)

add_executable(rpc-kv-db kv-db.cc database.cc)
target_link_libraries(rpc-kv-db rpc-kv-db-proto mimosa-rpc)

add_executable(rpc-kv-client kv-db-client.cc)
target_link_libraries(rpc-kv-client rpc-kv-db-proto mimosa-rpc)
