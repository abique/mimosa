#ifndef MIMOSA_CONTAINER_SINGLETON_HH
# define MIMOSA_CONTAINER_SINGLETON_HH

# include "../sync/spinlock.hh"

namespace mimosa
{
  namespace container
  {
    /**
     * @warning NOT THREAD SAFE
     * Helpers to implement a singleton.
     * Though, you should follow the @see initialisation rule.
     */
    template <typename T>
    class Singleton
    {
    public:
      static T & instance();
      static void release();

    private:
      static T *      instance_;
    };
  }
}

# include "singleton.hxx"

#endif /* !MIMOSA_CONTAINER_SINGLETON_HH */
