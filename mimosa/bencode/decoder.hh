#ifndef MIMOSA_BENCODE_DECODER_HH
# define MIMOSA_BENCODE_DECODER_HH

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
      Decoder(stream::Stream::Ptr input);

      Token pull(Time timeout = 0);
      bool  eatValue(Time timeout = 0);

      int64_t getInt();
      std::string & getData();

    private:
      Token pullInt(Time timeout);
      Token pullData(Time timeout);

      stream::Stream::Ptr input_;
      std::string         data_;
      int64_t             int_;
    };
  }
}

#endif /* !MIMOSA_BENCODE_DECODER_HH */
