#pragma once

namespace mimosa
{
  class NonCopyable
  {
  protected:
    constexpr NonCopyable() = default;

    // allow move stuff
    inline NonCopyable(NonCopyable &&) {}
    inline NonCopyable & operator=(NonCopyable&&) { return *this; }

  private:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
  };
}
