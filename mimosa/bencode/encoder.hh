#ifndef MIMOSA_BENCODE_ENCODER_HH
# define MIMOSA_BENCODE_ENCODER_HH

# include "../string/string-ref.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace bencode
  {
    class Encoder
    {
    public:
      Encoder(stream::Stream::Ptr output);

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

    private:
      stream::Stream::Ptr output_;
    };
  }
}

#endif /* !MIMOSA_BENCODE_ENCODER_HH */
