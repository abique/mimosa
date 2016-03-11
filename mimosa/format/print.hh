#ifndef MIMOSA_FORMAT_PRINT_HH
# define MIMOSA_FORMAT_PRINT_HH

# include <cstdint>
# include <string>

# include "../time.hh"
# include "../stream/stream.hh"
# include "../string-ref.hh"

namespace mimosa
{
  namespace format
  {
    template <typename T>
    inline bool printDecimal(stream::Stream & stream, T value);

    bool printHex(stream::Stream & stream, uint64_t value);
    /** will print the size with a precision of 3 digits */
    bool printByteSize(stream::Stream & stream, uint64_t value);

    bool printDuration(stream::Stream & stream, Time time);

    template <size_t len>
    inline bool printStatic(stream::Stream & stream, const char (&str)[len]);
    inline bool print(stream::Stream & stream, const char *str, size_t len);
    inline bool print(stream::Stream & stream, const char *str);
    inline bool print(stream::Stream & stream, const std::string & str);
    inline bool print(stream::Stream & stream, const StringRef & str);
  }
}

# include "print.hxx"

#endif /* !MIMOSA_FORMAT_PRINT_HH */
