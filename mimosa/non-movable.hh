#pragma once

namespace mimosa
{
  class NonMovable
  {
  protected:
    constexpr NonMovable() noexcept = default;

    inline NonMovable(NonMovable &&) = delete;
    inline NonMovable & operator=(NonMovable&&) = delete;
  };
}
