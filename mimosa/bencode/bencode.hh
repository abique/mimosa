#ifndef MIMOSA_BENCODE_BENCODE_HH
# define MIMOSA_BENCODE_BENCODE_HH

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

    struct Encoder
    {
      bool pushInt(int64_t value, runtime::Time timeout = 0);
      bool pushData(const char *data, size_t len, runtime::Time timeout = 0);

      inline bool pushData(const std::string & data, runtime::Time timeout = 0) {
        return pushData(data.data(), data.size(), timeout);
      }

      inline bool pushData(const string::StringRef & data, runtime::Time timeout = 0) {
        return pushData(data.data(), data.size(), timeout);
      }

      bool startDict(runtime::Time timeout = 0);
      bool startList(runtime::Time timeout = 0);
      bool end(runtime::Time timeout = 0);

      stream::Stream::Ptr output_;
    };
  }
}

#endif /* !MIMOSA_BENCODE_BENCODE_HH */
