#include "decoder.hh"

namespace mimosa
{
  namespace json
  {
    Decoder::Decoder(stream::Stream::Ptr input)
      : input_(input)
    {
    }

    Decoder::Token
    Decoder::pull()
    {
      int ret;
      char c;

      ret = input_->loopRead(&c, 1);
      if (ret < 0)
        return kReadError;
      if (ret == 0)
        return kEof;

      switch (c) {
      case ',':
        return pull();

      case '[':
        return kArrayBegin;

      case ']':
        return kArrayEnd;

      case '{':
        return kObjectBegin;

      case '}':
        return kObjectEnd;

      case '"':
        return pullString();

      case 'n':
        return pullNull();

      case 't':
        return pullTrue();

      case 'f':
        return pullFalse();

      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return pullNumber();

      default:
        return kParseError;
      }
    }
  }
}
