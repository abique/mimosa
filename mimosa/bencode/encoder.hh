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
    class Encoder
    {
    public:
      Encoder(stream::Stream::Ptr output);

      bool pushInt(int64_t value);
      bool pushData(const char *data, size_t len);

      inline bool pushData(const std::string & data) {
        return pushData(data.data(), data.size());
      }

      inline bool pushData(const StringRef & data) {
        return pushData(data.data(), data.size());
      }

      bool startDict();
      bool startList();
      bool end();

    private:
      stream::Stream::Ptr output_;
    };
  }
}

