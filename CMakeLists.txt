cmake_minimum_required(VERSION 2.8)

project(mimosa C CXX)

enable_testing()

find_package(GnuTLS REQUIRED)

include_directories(${GNUTLS_INCLUDE_DIR} .)

# -Weffc++
add_definitions(
  -std=c++0x
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DGOOGLE_PROTOBUF_NO_RTTI
  -Wall -Wextra -Werror=return-type
  -fsplit-stack -fno-exceptions -fno-rtti
  ${GNUTLS_DEFINITIONS})

include(cmake/Tests.cmake)
include(cmake/Flex.cmake)
include(cmake/Bison.cmake)
include(cmake/TemplateConverter.cmake)

add_subdirectory(mimosa)
