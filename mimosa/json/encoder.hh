#ifndef MIMOSA_JSON_ENCODER_HH
# define MIMOSA_JSON_ENCODER_HH

# include <vector>

# include "../stream/stream.hh"

namespace mimosa
{
  namespace json
  {
    /**
     * @ingroup json
     * JSON stream encoder.
     */
    class Encoder
    {
    public:
      Encoder(stream::Stream::Ptr output);

      bool startObject();
      bool endObject();

      bool startArray();
      bool endArray();

      bool pushString(const std::string & data);
      bool pushNumber(int64_t value);
      bool pushBoolean(bool value);
      bool pushNull();

    private:
      bool pushSeparator();
      void nextState();

      enum State
      {
        kArray,
        kArrayNext,
        kObjectKey,
        kObjectValue,
        kObjectNext,
      };

      stream::Stream::Ptr output_;
      std::vector<State>  state_;
    };

    class SyntaxError
    {
    };
  }
}

#endif /* !MIMOSA_JSON_ENCODER_HH */
