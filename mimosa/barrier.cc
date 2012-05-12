#include <cassert>

#include "barrier.hh"

namespace mimosa
{
  Barrier::Barrier()
    : mutex_(),
      cond_(),
      count_(0)
  {
  }

  Barrier::~Barrier()
  {
    wait();
  }

  void
  Barrier::wait()
  {
    Mutex::Locker locker(mutex_);
    if (count_ > 0)
      cond_.wait(mutex_);
  }

  Barrier::Handle
  Barrier::add()
  {
    Mutex::Locker locker(mutex_);
    ++count_;
    return Handle(*this);
  }

  Barrier::Handle::Handle(Barrier & barrier)
    : barrier_(&barrier)
  {
  }

  Barrier::Handle::Handle(Handle && handle)
    : barrier_(handle.barrier_)
  {
    handle.barrier_ = nullptr;
  }

  Barrier::Handle &
  Barrier::Handle::operator=(Handle && handle)
  {
    std::swap(barrier_, handle.barrier_);
    return *this;
  }

  Barrier::Handle::~Handle()
  {
    if (!barrier_)
      return;

    Mutex::Locker locker(barrier_->mutex_);
    if (--barrier_->count_ == 0)
      barrier_->cond_.wakeAll();
  }

  void
  Barrier::Handle::wait()
  {
    assert(barrier_);

    Mutex::Locker locker(barrier_->mutex_);
    if (--barrier_->count_ == 0)
      barrier_->cond_.wakeAll();
    else
      barrier_->cond_.wait(barrier_->mutex_);
  }
}
