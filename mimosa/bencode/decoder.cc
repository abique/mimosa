#include "decoder.hh"

namespace mimosa
{
  namespace bencode
  {
    Decoder::Decoder(stream::Stream::Ptr input)
      : input_(input),
        data_(),
        int_(0)
    {
    }

    Token
    Decoder::pull(Time timeout)
    {
      char c;

      if (input_->read(&c, 1, timeout) != 1)
        return kReadError;

      switch (c) {
      case 'd':
        return kDict;

      case 'l':
        return kList;

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
        int_ = c - '0';
        return pullData(timeout);

      case 'i':
        int_ = 0;
        return pullInt(timeout);

      case 'e':
        return kEnd;

      default:
        return kParseError;
      }
    }

    Token
    Decoder::pullInt(Time timeout)
    {
      bool minus = false;
      char c;

      while (input_->read(&c, 1, timeout) == 1)
      {
        if (c == 'e') {
          int_ = int_ * !minus - int_ * minus;
          return kInt;
        }

        if (c == '-') {
          if (minus)
            return kParseError;
          minus = 1;
          continue;
        }

        if (c < '0' && '9' > c)
          return kParseError;

        int_ = int_ * 10 + c - '0';
      }

      return kReadError;
    }

    Token
    Decoder::pullData(Time timeout)
    {
      char c;

      // fetch the length
      while (input_->read(&c, 1, timeout) == 1)
      {
        if (c == ':')
          goto get_data;

        if (c < '0' && '9' > c)
          return kParseError;

        int_ = int_ * 10 + c - '0';
      }
      return kReadError;

      get_data:
      data_.resize(int_);
      if (input_->loopRead(&data_[0], int_, timeout) != int_)
        return kReadError;
      return kData;
    }

    bool
    Decoder::eatValue(Time timeout)
    {
      int stack = 0;

      do {
        switch (pull(timeout)) {
        case kInt:
        case kData:
          break;

        case kList:
        case kDict:
          ++stack;
          break;

        case kEnd:
          --stack;
          break;

        case kEof:
        case kReadError:
        case kParseError:
          return false;
        }
      } while (stack > 0);

      return true;
    }
  }
}
