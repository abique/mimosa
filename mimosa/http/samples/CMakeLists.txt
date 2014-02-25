add_custom_target(key.pem certtool -p --outfile key.pem)
add_custom_target(cert.pem certtool -s --load-privkey key.pem --outfile cert.pem
  DEPENDS key.pem)

add_executable(web-server web-server.cc)
target_link_libraries(web-server mimosa-http)

add_executable(mget mget.cc)
target_link_libraries(mget mimosa-http)
