cmake_minimum_required(VERSION 2.8)

project(mimosa C CXX)

enable_testing()

find_package(ZLIB)
find_package(GnuTLS)

include_directories(${GNUTLS_INCLUDE_DIR} .)

add_definitions(
  -std=c++11
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
  -Wall -Wextra -Werror=return-type
  # -Weffc++
  # -fsplit-stack
)

if(GNUTLS_FOUND)
  add_definitions(${GNUTLS_DEFINITIONS})
endif()

include(cmake/Tests.cmake)
include(cmake/Flex.cmake)
include(cmake/Bison.cmake)
include(cmake/TemplateConverter.cmake)

include(CheckFunctionExists)

check_function_exists(strchrnul HAS_STRCHRNUL)
if(HAS_STRCHRNUL)
  add_definitions(-DHAS_STRCHRNUL)
endif()

check_function_exists(timegm HAS_TIMEGM)
if(HAS_TIMEGM)
  add_definitions(-DHAS_TIMEGM)
endif()

add_subdirectory(mimosa)
