#ifndef MIMOSA_BARRIER_HH
# define MIMOSA_BARRIER_HH

# include <stdexcept>

# include "non-copyable.hh"
# include "ref-countable.hh"
# include "mutex.hh"
# include "condition.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class Barrier : public RefCountable<Barrier>,
                  private NonCopyable
  {
  public:
    Barrier();
    ~Barrier();

    class Handle : public NonCopyable
    {
    public:
      Handle(Handle && handle);
      ~Handle();

      Handle & operator=(Handle && handle);

      void wait();

    private:
      friend class Barrier;

      Handle(Barrier & barrier);

      Barrier::Ptr barrier_;
    };

    Handle add();
    void   wait();

  private:
    friend class Handle;

    Mutex     mutex_;
    Condition cond_;
    uint32_t  count_;
  };
}

#endif /* !MIMOSA_BARRIER_HH */
