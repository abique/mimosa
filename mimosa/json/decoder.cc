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
    Decoder::pullNull()
    {
      int  ret;
      char buffer[3];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret != sizeof (buffer))
        throw ReadError;
      if (buffer[0] != 'u' || buffer[1] != 'l' || buffer[2] != 'l')
        throw SyntaxError;
      return kNull;
    }

    Decoder::Token
    Decoder::pullTrue()
    {
      int  ret;
      char buffer[3];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret != sizeof (buffer))
        throw ReadError;
      if (buffer[0] != 'r' || buffer[1] != 'u' || buffer[2] != 'e')
        throw SyntaxError;
      boolean_ = true;
      return kBoolean;
    }

    Decoder::Token
    Decoder::pullFalse()
    {
      int  ret;
      char buffer[4];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret != sizeof (buffer))
        throw ReadError;
      if (buffer[0] != 'a' || buffer[1] != 'l' || buffer[2] != 's' || buffer[3] != 'e')
        throw SyntaxError;
      boolean_ = false;
      return kBoolean;
    }

    Decoder::Token
    Decoder::pullString()
    {
      char c;

      string_.clear();
      while (input_->loopRead(&c, sizeof (c)) == 1) {
        if (c == '"')
          return kString;
        if (c != '\\') {
          string_.append(c);
          continue;
        }

        if (input_->loopRead(&c, sizeof (c)) != 1)
          throw ReadError;
        switch (c) {
        case '"': string_.append('"'); break;
        case '\\': string_.append('\\'); break;
        case '/': string_.append('/'); break;
        case 'b': string_.append('\b'); break;
        case 'f': string_.append('\f'); break;
        case 'n': string_.append('\n'); break;
        case 'r': string_.append('\r'); break;
        case 't': string_.append('\t'); break;
        case 'u':
          char u[4];
          if (input_->loopRead(u, sizeof (u)) != 4)
            throw ReadError;
          // XXX
          break;

        default:
          throw SyntaxError;
        }
      }
      throw ReadError;
    }

    Decoder::Token
    Decoder::pullValue()
    {
      Decoder::Token token;
      int            ret;
      char           c;

      ret = input_->loopRead(&c, 1);
      if (ret < 0)
        throw ReadError;
      if (ret == 0) {
        if (!state_.empty())
          throw SyntaxError;
        return kEof;
      }

      switch (c) {
      case '[':
        state_.push_back(kArray);
        return kArrayBegin;

      case '{':
        state_.push_back(kObjectKey);
        return kObjectBegin;

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
        throw SyntaxError;
      }
    }

    Decoder::Token
    Decoder::pull()
    {
      if (state_.empty())
        return pullValue();

      auto & state = state_.back();
      switch (state) {
      case kArray:
        token = pullValueOrArrayEnd();
        if (token == kArrayEnd)
          state_.pop_back();
        else
          state = kArrayNext;
        return token;

      case kArrayNext:
        token = pullCommaOrArrayEnd();
        if (token == kArrayEnd)
          state_.pop_back();
        return token;

      case kObjectKey:
        token = pullObjectKey();
        if (token == kObjectEnd)
          state_.pop_back();
        else
          state = kObjectValue;
        return token;

      case kObjectValue:
        return pullObjectValue();

      case kObjectNext:
        token = pullObjectNext();
        if (token == kObjectEnd)
          state_.pop_back();
        else
          state = kObjectValue;
        return token;
      }
    }
  }
}
