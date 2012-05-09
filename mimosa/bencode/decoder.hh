#ifndef MIMOSA_BENCODE_DECODER_HH
# define MIMOSA_BENCODE_DECODER_HH

# include "../string/string-ref.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace bencode
  {
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

    class Decoder
    {
    public:
      Decoder(stream::Stream::Ptr input);

      Token pull(runtime::Time timeout = 0);
      bool  eatValue(runtime::Time timeout = 0);

      int64_t getInt();
      std::string & getData();

    private:
      Token pullInt(runtime::Time timeout);
      Token pullData(runtime::Time timeout);

      stream::Stream::Ptr input_;
      std::string         data_;
      int64_t             int_;
    };
  }
}

#endif /* !MIMOSA_BENCODE_DECODER_HH */
