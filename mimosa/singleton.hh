#ifndef MIMOSA_SINGLETON_HH
# define MIMOSA_SINGLETON_HH

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

  private:
    static T * instance_;
  };
}

# include "singleton.hxx"

#endif /* !MIMOSA_SINGLETON_HH */
