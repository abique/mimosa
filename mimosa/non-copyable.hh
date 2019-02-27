#pragma once

namespace mimosa
{
  class NonCopyable
  {
  protected:
    constexpr NonCopyable() noexcept = default;

    // allow move stuff
    inline NonCopyable(NonCopyable &&) noexcept {}
    inline NonCopyable & operator=(NonCopyable&&) noexcept { return *this; }

  private:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
  };
}
