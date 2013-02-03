#ifndef MIMOSA_JSON_ENCODER_HH
# define MIMOSA_JSON_ENCODER_HH

namespace mimosa
{
  namespace json
  {
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
      enum State
      {
        kArray,
        kObjectKey,
        kObjectValue,
      };

      stream::Stream::Ptr output_;
      std::vector<State>  state_;
    };
  }
}

#endif /* !MIMOSA_JSON_ENCODER_HH */
