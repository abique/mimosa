#ifndef MIMOSA_FORMAT_PRINT_HH
# define MIMOSA_FORMAT_PRINT_HH

# include <cstdint>
# include <string>

# include "../time.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace format
  {
    template <typename T>
    inline bool printDecimal(stream::Stream & stream, T value, Time timeout);

    bool printHex(stream::Stream & stream, uint64_t value, Time timeout);
    /** will print the size with a precision of 3 digits */
    bool printByteSize(stream::Stream & stream, uint64_t value, Time timeout);

    bool printDuration(stream::Stream & stream, Time time, Time timeout);

    template <size_t len>
    inline bool printStatic(stream::Stream & stream, const char (&str)[len], Time timeout);
    inline bool print(stream::Stream & stream, const char *str, size_t len, Time timeout);
    inline bool print(stream::Stream & stream, const char *str, Time timeout);
    inline bool print(stream::Stream & stream, const std::string & str, Time timeout);
  }
}

# include "print.hxx"

#endif /* !MIMOSA_FORMAT_PRINT_HH */
