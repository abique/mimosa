#ifndef MIMOSA_JSON_DECODER_HH
# define MIMOSA_JSON_DECODER_HH

#include <vector>

#include "../stream/stream.hh"

namespace mimosa
{
  namespace json
  {
    /**
     * @ingroup json
     * JSON stream decoder.
     */
    class Decoder
    {
    public:
      Decoder(stream::Stream::Ptr input);

      enum Token
      {
        /* real tokens */
        kArrayBegin,
        kArrayEnd,
        KObjectBegin,
        kObjectEnd,
        kString,
        kNumber,
        kBoolean,
        kNull,
        kEof,

        /* error tokens */
        kReadError,
        kParseError,
      };

      Token pull();

      inline std::string & string() const
      {
        return string_;
      }

      inline int64_t number() const
      {
        return number_;
      }

      inline bool boolean() const
      {
        return boolean_;
      }

    private:
      enum State
      {
        kArray,
        kArrayNext,
        kObjectKey,
        kObjectValue,
        kObjectNext,
      };

      stream::Stream::Ptr input_;
      std::vector<State>  state_;

      int64_t     number_;
      bool        boolean_;
      std::string string_;
    };
  }
}

#endif /* !MIMOSA_JSON_DECODER_HH */
