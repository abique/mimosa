#pragma once

#include <vector>
#include <string>

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
      explicit Decoder(stream::Stream::Ptr input);

      enum Token
      {
        /* real tokens */
        kArrayBegin,
        kArrayEnd,
        kObjectBegin,
        kObjectEnd,
        kString,
        kInteger,
        kRational,
        kBoolean,
        kNull,
        kEof,
      };

      struct ReadError {};
      struct PrematureEof {};
      struct SyntaxError {};

      Token pull();
      void  eatValue();

      inline const std::string & string() const
      {
        return string_;
      }

      inline int64_t integer() const
      {
        return integer_;
      }

      inline double rational() const
      {
        return rational_;
      }

      inline bool boolean() const
      {
        return boolean_;
      }

      inline size_t depth() const
      {
        return state_.size();
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
      enum Token pullRationalDot(int sign);
      enum Token pullRationalExp();
      enum Token pullValue();
      enum Token pullValueOrArrayEnd();
      enum Token pullCommaOrArrayEnd();
      enum Token pullObjectKey();
      enum Token pullObjectValue();
      enum Token pullObjectNext();

      bool getc(char * c);
      bool getcnows(char * c);
      void ungetc(char c);

      stream::Stream::Ptr input_;
      std::vector<State>  state_;

      double      rational_;
      int64_t     integer_;
      bool        boolean_;
      std::string string_;

      bool has_c_; // true if we have a character in c_
      char c_;     // buffer for ungetc
    };
  }
}

