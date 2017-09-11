#pragma once

#include <stdexcept>

#include "non-copyable.hh"
#include "non-movable.hh"
#include "ref-countable.hh"
#include "mutex.hh"
#include "condition.hh"

namespace mimosa
{
  /**
   * @ingroup Sync
   */
  class Barrier : public RefCountable<Barrier>, private NonCopyable, private NonMovable
  {
  public:
    Barrier();
    ~Barrier();

    class Handle : private mimosa::NonCopyable, private mimosa::NonMovable
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

