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
    Decoder::pull()
    {
      char c;

      if (input_->read(&c, 1) != 1)
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
        return pullData();

      case 'i':
        int_ = 0;
        return pullInt();

      case 'e':
        return kEnd;

      default:
        return kParseError;
      }
    }

    Token
    Decoder::pullInt()
    {
      bool minus = false;
      char c;

      while (input_->read(&c, 1) == 1)
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
    Decoder::pullData()
    {
      char c;

      // fetch the length
      while (input_->read(&c, 1) == 1)
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
      if (input_->loopRead(&data_[0], int_) != int_)
        return kReadError;
      return kData;
    }

    bool
    Decoder::eatValue()
    {
      int stack = 0;

      do {
        switch (pull()) {
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
