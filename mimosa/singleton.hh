#pragma once

#include <atomic>

namespace mimosa
{
  /**
   * @warning NOT THREAD SAFE, you need to manualy call release to ensure
   * resources liberation.
   *
   * Helpers to implement a singleton.
   * Though, you should follow the @ref initialisation rule.
   */
  template <typename T>
  class Singleton
  {
  public:
    static T & instance();
    static void release();

  protected:
    static std::atomic<T *> instance_;
  };
}

# include "singleton.hxx"

