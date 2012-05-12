#ifndef MIMOSA_BENCODE_ENCODER_HH
# define MIMOSA_BENCODE_ENCODER_HH

# include "../string-ref.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace bencode
  {
    /**
     * @ingroup bencode
     */
    class Encoder
    {
    public:
      Encoder(stream::Stream::Ptr output);

      bool pushInt(int64_t value, Time timeout = 0);
      bool pushData(const char *data, size_t len, Time timeout = 0);

      inline bool pushData(const std::string & data, Time timeout = 0) {
        return pushData(data.data(), data.size(), timeout);
      }

      inline bool pushData(const StringRef & data, Time timeout = 0) {
        return pushData(data.data(), data.size(), timeout);
      }

      bool startDict(Time timeout = 0);
      bool startList(Time timeout = 0);
      bool end(Time timeout = 0);

    private:
      stream::Stream::Ptr output_;
    };
  }
}

#endif /* !MIMOSA_BENCODE_ENCODER_HH */
