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
        kInteger,
        kRational,
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

      inline int64_t integer() const
      {
        return integer_;
      }

      inline int64_t rational() const
      {
        return rational_;
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

      enum Token pullNull();
      enum Token pullTrue();
      enum Token pullFalse();
      enum Token pullString();
      enum Token pullNumber();

      stream::Stream::Ptr input_;
      std::vector<State>  state_;

      double      rational_;
      int64_t     integer_;
      bool        boolean_;
      std::string string_;
    };
  }
}

#endif /* !MIMOSA_JSON_DECODER_HH */
