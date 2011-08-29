add_custom_target(key.pem certtool -p --outfile key.pem)
add_custom_target(cert.pem certtool -s --load-privkey key.pem --outfile cert.pem
  DEPENDS key.pem)

add_executable(web-server EXCLUDE_FROM_ALL web-server.cc)
target_link_libraries(web-server mimosa-http)
