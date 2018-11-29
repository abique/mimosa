#include <iostream>
#include <cmath>
#include <limits>
#include <utility>
#include <utility>

#include "decoder.hh"

namespace mimosa
{
  namespace json
  {
    Decoder::Decoder(stream::Stream::Ptr input)
      : input_(std::move(std::move(input))),
        has_c_(false)
    {
    }

    Decoder::Token
    Decoder::pullNull()
    {
      int  ret;
      char buffer[3];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret < 0)
        throw ReadError();
      if (ret != sizeof (buffer))
        throw PrematureEof();
      if (buffer[0] != 'u' || buffer[1] != 'l' || buffer[2] != 'l')
        throw SyntaxError();
      return kNull;
    }

    Decoder::Token
    Decoder::pullTrue()
    {
      int  ret;
      char buffer[3];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret < 0)
        throw ReadError();
      if (ret != sizeof (buffer))
        throw PrematureEof();
      if (buffer[0] != 'r' || buffer[1] != 'u' || buffer[2] != 'e')
        throw SyntaxError();
      boolean_ = true;
      return kBoolean;
    }

    Decoder::Token
    Decoder::pullFalse()
    {
      int  ret;
      char buffer[4];

      ret = input_->loopRead(buffer, sizeof (buffer));
      if (ret < 0)
        throw ReadError();
      if (ret != sizeof (buffer))
        throw PrematureEof();
      if (buffer[0] != 'a' || buffer[1] != 'l' || buffer[2] != 's' || buffer[3] != 'e')
        throw SyntaxError();
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
          string_.append(1, c);
          continue;
        }

        if (input_->loopRead(&c, sizeof (c)) != 1)
          throw ReadError();
        switch (c) {
        case '"': string_.append(1, '"'); break;
        case '\\': string_.append(1, '\\'); break;
        case '/': string_.append(1, '/'); break;
        case 'b': string_.append(1, '\b'); break;
        case 'f': string_.append(1, '\f'); break;
        case 'n': string_.append(1, '\n'); break;
        case 'r': string_.append(1, '\r'); break;
        case 't': string_.append(1, '\t'); break;
        case 'u':
          char u[4];
          if (input_->loopRead(u, sizeof (u)) != 4)
            throw ReadError();
          // XXX
          break;

        default:
          throw SyntaxError();
        }
      }
      throw ReadError();
    }

    Decoder::Token
    Decoder::pullRationalExp()
    {
      int sign = 1;
      int64_t exp = 0;
      char c;
      bool got_number = false;

      while (true) {
        if (!getc(&c)) {
          if (!got_number)
            throw SyntaxError();
          goto ret;
        }

        switch (c) {
        case '-':
          if (sign == -1 || got_number)
            throw SyntaxError();
          sign = -1;
          break;

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
          got_number = true;
          exp = 10 * exp + c - '0';
          break;

        default:
          ungetc(c);
          goto ret;
        }
      }

      ret:
      rational_ *= pow(10, sign * exp);
      return kRational;
    }

    Decoder::Token
    Decoder::pullRationalDot(int sign)
    {
      char c;
      bool got_number = false;
      double div = 10;

      while (true) {
        if (!getc(&c)) {
          if (!got_number)
            throw SyntaxError();
          goto ret;
        }

        switch (c) {
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
          got_number = true;
          rational_ += (((double)(c - '0')) / div);
          div *= 10;
          break;

        case 'E':
        case 'e':
          if (!got_number)
            throw SyntaxError();
          rational_ *= sign;
          return pullRationalExp();

        default:
          if (!got_number)
            throw SyntaxError();
          ungetc(c);
          goto ret;
        }
      }

      ret:
      rational_ *= sign;
      return kRational;
    }

    Decoder::Token
    Decoder::pullNumber()
    {
      int sign = 1;
      char c;
      bool got_number = false;

      integer_ = 0;

      while (true) {
        if (!getc(&c)) {
          if (!got_number)
            throw SyntaxError();
          goto ret;
        }

        switch (c) {
        case '-':
          if (sign == -1 || got_number)
            throw SyntaxError();
          sign = -1;
          break;

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
          got_number = true;
          integer_ = 10 * integer_ + c - '0';
          break;

        case '.':
          if (!got_number)
            throw SyntaxError();
          rational_ = integer_;
          return pullRationalDot(sign);

        case 'E':
        case 'e':
          if (!got_number)
            throw SyntaxError();
          rational_ = integer_ * sign;
          return pullRationalExp();

        case 'i':
          if (got_number || !getc(&c) || c != 'n' || !getc(&c) || c != 'f')
            throw SyntaxError();
          rational_ = sign * std::numeric_limits<double>::infinity();
          return kRational;

        default:
          ungetc(c);
          goto ret;
        }
      }

      ret:
      integer_ *= sign;
      return kInteger;
    }

    Decoder::Token
    Decoder::pullValue()
    {
      char c;

      if (!getcnows(&c)) {
        if (!state_.empty())
          throw SyntaxError();
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
      case 'i':
        ungetc(c);
        return pullNumber();

      default:
        throw SyntaxError();
      }
    }

    Decoder::Token
    Decoder::pullValueOrArrayEnd()
    {
      char c;

      if (!getcnows(&c))
        throw SyntaxError();

      switch (c) {
      case ']':
        state_.pop_back();
        return kArrayEnd;

      default:
        ungetc(c);
        state_.back() = kArrayNext;
        return pullValue();
      }
    }

    Decoder::Token
    Decoder::pullCommaOrArrayEnd()
    {
      char c;

      if (!getcnows(&c))
        throw SyntaxError();

      switch (c) {
      case ']':
        state_.pop_back();
        return kArrayEnd;

      case ',':
        state_.back() = kArrayNext;
        return pullValue();

      default:
        throw SyntaxError();
      }
    }

    Decoder::Token
    Decoder::pullObjectKey()
    {
      char c;

      if (!getcnows(&c))
        throw SyntaxError();

      switch (c) {
      case '"':
        state_.back() = kObjectValue;
        return pullString();

      case '}':
        state_.pop_back();
        return kObjectEnd;

      default:
        throw SyntaxError();
      }
    }

    Decoder::Token
    Decoder::pullObjectValue()
    {
      char c;

      if (!getcnows(&c) || c != ':')
        throw SyntaxError();

      state_.back() = kObjectNext;
      return pullValue();
    }

    Decoder::Token
    Decoder::pullObjectNext()
    {
      char c;

      if (!getcnows(&c))
        throw PrematureEof();

      switch (c) {
      case ',':
        state_.back() = kObjectValue;
        if (!getcnows(&c))
          throw PrematureEof();
        if (c != '"')
          throw SyntaxError();
        return pullString();

      case '}':
        state_.pop_back();
        return kObjectEnd;

      default:
        throw SyntaxError();
      }
    }

    Decoder::Token
    Decoder::pull()
    {
      if (state_.empty())
        return pullValue();

      switch (state_.back()) {
      case kArray:
        return pullValueOrArrayEnd();

      case kArrayNext:
        return pullCommaOrArrayEnd();

      case kObjectKey:
        return pullObjectKey();

      case kObjectValue:
        return pullObjectValue();

      case kObjectNext:
        return pullObjectNext();

      default:
        assert(false);
        return kEof;
      }
    }

    bool
    Decoder::getc(char * c)
    {
      if (has_c_) {
        *c = c_;
        has_c_ = false;
        return true;
      }
      int ret = input_->loopRead(c, 1);
      if (ret < 0)
        throw ReadError();
      return ret == 1;
    }

    bool
    Decoder::getcnows(char * c)
    {
      while (true) {
        if (!getc(c))
          return false;

        if (!::isspace(*c))
          return true;
      }
    }

    void
    Decoder::ungetc(char c)
    {
      assert(!has_c_);
      c_     = c;
      has_c_ = true;
    }

    void
    Decoder::eatValue()
    {
      int depth = 0;

      do {
        auto token = pull();
        switch (token) {
        case kArrayBegin:
        case kObjectBegin:
          ++depth;
          break;

        case kArrayEnd:
        case kObjectEnd:
          --depth;
          break;

        case kString:
        case kRational:
        case kInteger:
        case kBoolean:
        case kNull:
          break;

        case kEof:
          return;
        }
      } while (depth > 0);
    }
  }
}
