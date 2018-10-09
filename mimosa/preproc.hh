#pragma once

#if defined(__linux) || defined(__linux__)
# define MIMOSA_LINUX
# define MIMOSA_UNIX
#elif defined(__APPLE__)
# define MIMOSA_MAC
# define MIMOSA_UNIX
#elif defined(__unix__) || defined(__HAIKU__)
# define MIMOSA_HAIKU
# define MIMOSA_UNIX
#elif defined(__WIN32__)
# define MIMOSA_WIN
#endif
