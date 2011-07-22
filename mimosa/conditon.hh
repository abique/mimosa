#ifndef MIMOSA_CONDITON_HH
# define MIMOSA_CONDITON_HH

# include "mutex.hh"

namespace mimosa
{
  class Condition : public NonCopyable
  {
  public:
    inline Condition() : cond_(::melon_cond_new()) {}
    inline ~Condition() { ::melon_cond_destroy(cond_); }
    inline void wait(Mutex & mutex) { ::melon_cond_wait(cond_, mutex.mutex_); }
    inline void timedWait(Mutex & mutex, ::melon_time_t time) { ::melon_cond_wait(cond_, mutex.mutex_, time); }
    inline void wakeOne() { ::melon_cond_signal(cond_); }
    inline void wakeAll() { ::melon_cond_broadcast(cond_); }
  private:
    ::melon_cond * cond_;
  };
}

#endif /* !MIMOSA_CONDITON_HH */
