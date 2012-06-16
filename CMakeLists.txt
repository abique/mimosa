cmake_minimum_required(VERSION 2.8)

project(mimosa C CXX)

enable_testing()

find_package(GnuTLS REQUIRED)

include_directories(${GNUTLS_INCLUDE_DIR} .)

add_definitions(
  -std=c++11
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE ${GNUTLS_DEFINITIONS}
  -Wall -Wextra -Werror=return-type
  # -Weffc++
  # -fsplit-stack
)

include(cmake/Tests.cmake)
include(cmake/Flex.cmake)
include(cmake/Bison.cmake)
include(cmake/TemplateConverter.cmake)

add_subdirectory(mimosa)
