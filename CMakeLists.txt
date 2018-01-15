cmake_minimum_required(VERSION 3.4)

project(mimosa C CXX)

enable_testing()

find_package(ZLIB)
find_package(GnuTLS)
find_package(LibArchive)

include_directories(${GNUTLS_INCLUDE_DIR} ${LibArchive_INCLUDE_DIR} .)

add_definitions(
  -std=c++14
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
  -Wall -Wextra -Werror=return-type
)

if(GNUTLS_FOUND)
  add_definitions(${GNUTLS_DEFINITIONS})
  add_definitions(${LibArchive_DEFINITIONS})
endif()

include(cmake/Tests.cmake)
include(cmake/Flex.cmake)
include(cmake/Bison.cmake)
include(cmake/ProtocMimosa.cmake)

include(CheckSymbolExists)

check_symbol_exists(writev sys/uio.h HAS_WRITEV)
if(HAS_WRITEV)
  add_definitions(-DHAS_WRITEV)
endif()

check_symbol_exists(strchrnul string.h HAS_STRCHRNUL)
if(HAS_STRCHRNUL)
  add_definitions(-DHAS_STRCHRNUL)
endif()

check_symbol_exists(timegm time.h HAS_TIMEGM)
if(HAS_TIMEGM)
  add_definitions(-DHAS_TIMEGM)
endif()

check_symbol_exists(sched_setaffinity sched.h HAS_SCHED_SETAFFINITY)
if(HAS_SCHED_SETAFFINITY)
  add_definitions(-DHAS_SCHED_SETAFFINITY)
endif()

check_symbol_exists(poll poll.h HAS_POLL)
if(HAS_POLL)
  add_definitions(-DHAS_POLL)
endif()

check_symbol_exists(pipe unistd.h HAS_PIPE)
if(HAS_PIPE)
  add_definitions(-DHAS_PIPE)
endif()

add_subdirectory(mimosa)
