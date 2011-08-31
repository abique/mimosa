add_executable(rpc-kv-db kv-db.cc kv-db.pb.cc)
target_link_libraries(rpc-kv-db mimosa-rpc)
