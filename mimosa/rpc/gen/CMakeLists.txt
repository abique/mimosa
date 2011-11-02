add_executable(protoc-gen-mimosa protoc-gen-mimosa.cc)
target_link_libraries(protoc-gen-mimosa protoc protobuf pthread z)

install(TARGETS protoc-gen-mimosa
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
