#ifndef MIMOSA_SYNC_SEMAPHORE_HANDLE_HH
# define MIMOSA_SYNC_SEMAPHORE_HANDLE_HH

namespace mimosa
{
  namespace sync
  {
    class SemaphoreHandle : private NonCopyable
    {
    public:
      inline SemaphoreHandle(Semaphore & sem, int nb)
        : sem_(sem),
          nb_(nb)
      {
        sem_.acquire(nb_);
      }

      inline ~SemaphoreHandle()
      {
        sem_.release(nb_);
      }

      inline void acquire(int nb = 1)
      {
        nb_ += nb;
        sem_.acquire(nb);
      }

      inline void release(int nb = 1) throw std::logic_error
      {
        if (nb > nb_)
          throw std::logic_error("releasing more resources than acquired");
        nb_ -= nb;
        sem_.release(nb);
      }

    private:
      Semaphore & sem_;
      int         nb_;
    };
  }
}

#endif /* !MIMOSA_SYNC_SEMAPHORE_HANDLE_HH */
