#pragma once

namespace mimosa
{
  class NonMovable
  {
  protected:
    constexpr NonMovable() = default;

    inline NonMovable(NonCopyable &&) = delete;
    inline NonMovable & operator=(NonCopyable&&) = delete;
  };
}
