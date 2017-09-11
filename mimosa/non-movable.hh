#pragma once

namespace mimosa
{
  class NonMovable
  {
  public:
    inline NonMovable() {}

    inline NonMovable(NonCopyable &&) = delete;
    inline NonMovable & operator=(NonCopyable&&) = delete;
  };
}
