#include <limits>
#include <utility>
#include <utility>

#include "encoder.hh"
#include "../format/format-stream.hh"

namespace mimosa
{
  namespace json
  {
    Encoder::Encoder(stream::Stream::Ptr output)
      : output_(std::move(std::move(output)))
    {
    }

    bool
    Encoder::startObject()
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if (output_->loopWrite("{", 1) != 1)
        return false;
      state_.push_back(kObjectKey);
      return true;
    }

    bool
    Encoder::endObject()
    {
      if (state_.empty() ||
          (state_.back() != kObjectKey && state_.back() != kObjectNext))
        throw SyntaxError();

      if (output_->loopWrite("}", 1) != 1)
        return false;

      state_.pop_back();
      nextState();
      return true;
    }

    bool
    Encoder::startArray()
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if (output_->loopWrite("[", 1) != 1)
        return false;

      state_.push_back(kArray);
      return true;
    }

    bool
    Encoder::endArray()
    {
      if (state_.empty() || (state_.back() != kArray && state_.back() != kArrayNext))
        throw SyntaxError();

      if (output_->loopWrite("]", 1) != 1)
        return false;

      state_.pop_back();
      nextState();
      return true;
    }

    bool
    Encoder::pushBoolean(bool value)
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if ((value && output_->loopWrite("true", 4) != 4) ||
          (!value && output_->loopWrite("false", 5) != 5))
        return false;

      nextState();
      return true;
    }

    bool
    Encoder::pushNull()
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if (output_->loopWrite("null", 4) != 4)
        return false;

      nextState();
      return true;
    }

    bool
    Encoder::pushString(const std::string_view & data)
    {
      if (!pushSeparator())
        return false;

      if (output_->loopWrite("\"", 1) != 1)
        return false;

      for (const char & it : data) {
        if (it == '\\' || it == '"')
          if (output_->loopWrite("\\", 1) != 1)
            return false;

        switch (it) {
        case '\r':
          if (output_->loopWrite("\\r", 2) != 2)
            return false;
          break;

        case '\n':
          if (output_->loopWrite("\\n", 2) != 2)
            return false;
          break;

        case '\t':
          if (output_->loopWrite("\\t", 2) != 2)
            return false;
          break;

        case '\v':
          if (output_->loopWrite("\\v", 2) != 2)
            return false;
          break;

        case '\b':
          if (output_->loopWrite("\\b", 2) != 2)
            return false;
          break;

        default:
          if (output_->loopWrite(&it, 1) != 1)
            return false;
        }
      }

      if (output_->loopWrite("\"", 1) != 1)
        return false;

      nextState();
      return true;
    }

    bool
    Encoder::pushNumber(int64_t value)
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if (!format::format(*output_, "%d", value))
        return false;

      nextState();
      return true;
    }

    bool
    Encoder::pushFloat(double value)
    {
      if (!state_.empty() && state_.back() == kObjectKey)
        throw SyntaxError();

      if (!pushSeparator())
        return false;

      if (value == std::numeric_limits<double>::infinity()) {
        if (!format::format(*output_, "\"inf\""))
          return false;
      } else if (value == -std::numeric_limits<double>::infinity()) {
        if (!format::format(*output_, "\"-inf\""))
          return false;
      }
      else if (!format::format(*output_, "%v", value))
        return false;

      nextState();
      return true;
    }

    bool
    Encoder::pushSeparator()
    {
      if (state_.empty())
        return true;

      switch (state_.back()) {
      case kArray:
      case kObjectKey:
        return true;

      case kArrayNext:
        if (output_->loopWrite(",", 1) != 1)
          return false;
        state_.back() = kArray;
        return true;

      case kObjectNext:
        if (output_->loopWrite(",", 1) != 1)
          return false;
        state_.back() = kObjectKey;
        return true;

      case kObjectValue:
        return output_->loopWrite(":", 1) == 1;

      default:
        return true;
      }
    }

    void
    Encoder::nextState()
    {
      if (state_.empty())
        return;

      switch (state_.back()) {
      case kArray:
        state_.back() = kArrayNext;
        break;

      case kArrayNext:
        break;

      case kObjectKey:
        state_.back() = kObjectValue;
        break;

      case kObjectValue:
        state_.back() = kObjectNext;
        break;

      case kObjectNext:
        state_.back() = kObjectKey;
        break;

      default:
        break;
      }
    }
  }
}
