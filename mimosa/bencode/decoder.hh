#pragma once

# include "../string-ref.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace bencode
  {
    /**
     * @ingroup bencode
     */
    enum Token
    {
      /* real token */
      kInt,
      kData,
      kList,
      kDict,
      kEnd,
      kEof,

      /* error token */
      kReadError,
      kParseError,
    };

    /**
     * @ingroup bencode
     */
    class Decoder
    {
    public:
      explicit Decoder(stream::Stream::Ptr input);

      Token pull();
      bool  eatValue();

      inline int64_t getInt() const { return int_; }
      inline const std::string & getData() const { return data_; }

      inline void setInput(stream::Stream::Ptr input) { input_ = input; }

    private:
      Token pullInt();
      Token pullData();

      stream::Stream::Ptr input_;
      std::string         data_;
      int64_t             int_;
    };
  }
}

